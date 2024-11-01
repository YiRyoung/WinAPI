#pragma once
#include <string>
#include <EngineBase/EngineMath.h>

namespace UEngineDebug
{
	// MainWindow�� �ִ�.

	void SetIsDebug(bool _IsDebug);

	void SwitchIsDebug();

	void CoreOutPutString(std::string_view _Text);

	void CoreOutPutString(std::string_view _Text, FVector2D _Pos);

	// �и��ؾ� �ϰڳ׿�

	void PrintEngineDebugText();
}