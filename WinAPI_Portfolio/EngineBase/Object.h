#pragma once
#include <string>

// 설명 :
class UObject
{
public:
	// constrcuter destructer
	UObject();

	// 혹여나 자식들의 소멸자가 호출 안되는 경우를 막기위에서
	// 가상함수 테이블을 만들 것이므로 왠만하면 자식쪽의 소멸자가 호출안되는 경우는 없을 것이다.
	virtual ~UObject();

	// delete Function
	UObject(const UObject& _Other) = delete;
	UObject(UObject&& _Other) noexcept = delete;
	UObject& operator=(const UObject& _Other) = delete;
	UObject& operator=(UObject&& _Other) noexcept = delete;

	std::string GetName() const
	{
		return Name;
	}

	std::string_view GetNameView() const
	{
		return Name.c_str();
	}

	// 이름 지정할때 뭔가 하고 싶으면 오버라이드해.
	virtual void SetName(std::string_view _Name)
	{
		Name = _Name.data();
	}

protected:

private:
	std::string Name;

};

