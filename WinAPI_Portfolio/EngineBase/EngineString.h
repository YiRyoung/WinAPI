#pragma once

// Ό³Έν :
class UEngineString
{
public:
	// constrcuter destructer
	~UEngineString();

	// delete Function
	UEngineString(const UEngineString& _Other) = delete;
	UEngineString(UEngineString&& _Other) noexcept = delete;
	UEngineString& operator=(const UEngineString& _Other) = delete;
	UEngineString& operator=(UEngineString&& _Other) noexcept = delete;

	static std::string ToUpper(std::string_view _string);

	static std::wstring AnsiToUnicode(std::string_view _Name);

protected:

private:
	UEngineString();

};

