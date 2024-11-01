#pragma once
#include <filesystem>

class UEnginePath
{
public:
		UEnginePath();
	UEnginePath(std::string_view _Path);
	UEnginePath(std::filesystem::path _Path);
	~UEnginePath();

					
	bool IsExists();
	void MoveParent();

	std::string GetPathToString();

		std::string GetFileName();

		std::string GetDirectoryName();

		std::string GetExtension();


								bool MoveParentToDirectory(std::string_view _Path);

	bool IsDirectory();

	bool IsFile();

	void Append(std::string_view _AppendName);


protected:
	std::filesystem::path Path;

private:
};

