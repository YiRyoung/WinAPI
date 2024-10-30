#include "PreCompile.h"
#include "EnginePath.h"
#include "EngineDebug.h"

UEnginePath::UEnginePath() : Path(std::filesystem::current_path())
{
}

UEnginePath::UEnginePath(std::string_view _Path) : Path(_Path)
{
}

UEnginePath::UEnginePath(std::filesystem::path _Path) : Path(_Path)
{
}

UEnginePath::~UEnginePath()
{
}

std::string UEnginePath::GetPathToString()
{
    return Path.string();
}

std::string UEnginePath::GetFileName()
{
    return Path.filename().string();
}

std::string UEnginePath::GetExtension()
{
    return Path.extension().string();
}

bool UEnginePath::IsExists()
{
    return std::filesystem::exists(Path);
}

bool UEnginePath::IsFile()
{
    return false == IsDirectory();
}

void UEnginePath::MoveParent()
{
    Path = Path.parent_path();
}

bool UEnginePath::MoveParentToDirectory(std::string_view _Path)
{
    UEnginePath DummyPath = UEnginePath(Path);

    if (false == DummyPath.IsDirectory())
    {
        MSGASSERT("디렉토리 경로일 때에만 MoveParentToDirectory를 호출할 수 있습니다.");
        return false;
    }

    bool Result = false;
    std::filesystem::path CurPath = DummyPath.Path;
    std::filesystem::path Root = CurPath.root_path();

    while (true)
    {
        CurPath = DummyPath.Path;

        if (CurPath == Root)
        {
            break;
        }

        CurPath.append(_Path);
        if (true == std::filesystem::exists(CurPath))   // 대상을 찾았다면
        {
            Result = true;
            Path = CurPath;
            break;
        }
        DummyPath.MoveParent(); // 찾지 못했다면 상위 폴더로 이동
    }
    return Result;
}

bool UEnginePath::IsDirectory()
{
    return std::filesystem::is_directory(Path);
}
