#pragma once
#include <functional>

class EngineDelegate	// �Լ� ���� ���� ����
{
public:
	// constrcuter destructer
	EngineDelegate();

	EngineDelegate(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}
	~EngineDelegate();

	// �Լ��� ��ü�� �����ϴ��� ���� Ȯ��
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
	// �Լ� �����͸� ������ ����Ʈ ����
	std::list<std::function<void()>> Functions;
};

