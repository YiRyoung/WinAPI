#pragma once
#include <filesystem>

class UEnginePath
{
public:
	// constrcuter destructer
	UEnginePath();
	UEnginePath(std::string_view _Path);
	UEnginePath(std::filesystem::path _Path);
	~UEnginePath();

	bool IsExists();	// 존재 여부 확인
	void MoveParent();	// 상위 폴더로 이동

	std::string GetPathToString();

	std::string GetFileName();	// 파일명 + 확장자 포함

	std::string GetDirectoryName();

	std::string GetExtension();	// 확장자만

	bool MoveParentToDirectory(std::string_view _Path);	// 상위 디렉토리로 이동
	bool IsDirectory();	// 디렉토리인지 확인
	bool IsFile();

	void Append(std::string_view _AppendName);

protected:
	std::filesystem::path Path;

private:

};

