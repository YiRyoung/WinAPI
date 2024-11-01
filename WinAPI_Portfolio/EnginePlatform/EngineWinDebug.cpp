#include "PreCompile.h"
#include "EngineWinDebug.h"


namespace UEngineDebug
{
	// f
	// 분리해야 하겠네요
	void WinAPIOutPutString(UEngineWinImage* _Image, std::string_view _Text, FVector2D _Pos)
	{
		// 쓰기가 귀찮고 어렵다.
		TextOutA(_Image->GetDC(), _Pos.iX(), _Pos.iY(), _Text.data(), static_cast<int>(_Text.size()));
	}
}