#pragma once
#include <functional>

class EngineDelegate	// 함수 여러 개를 실행
{
public:
	// constrcuter destructer
	EngineDelegate();

	EngineDelegate(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}
	~EngineDelegate();

	// 함수의 객체가 존재하는지 여부 확인
	bool IsBind()
	{
		return false == Functions.empty();
	}

	void operator+=(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}

	void operator()()
	{
		std::list<std::function<void()>>::iterator StartIter = Functions.begin();
		std::list<std::function<void()>>::iterator EndIter = Functions.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			std::function<void()>& Function = *StartIter;
			Function();
		}
	}

	void Clear()
	{
		Functions.clear();
	}

protected:

private:
	// 함수 포인터를 저장할 리스트 생성
	std::list<std::function<void()>> Functions;
};

