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

	bool MoveParentToDirectory(std::string_view _Path);	// 상위 디렉토리로 이동
	bool IsDirectory();	// 디렉토리인지 확인
	bool IsFile();
protected:
	std::filesystem::path Path;
private:

};

