#include "PreCompile.h"
#include "SpriteRenderer.h"
#include "ImageManager.h"

USpriteRenderer::USpriteRenderer()
{
}

USpriteRenderer::~USpriteRenderer()
{
}

void USpriteRenderer::Render()
{
}

void USpriteRenderer::BeginPlay()
{
}

void USpriteRenderer::ComponentTick(float _DeltaTime)
{
}

void USpriteRenderer::SetSprite(std::string_view _Name, int _CurIndex)
{
	Sprite = UImageManager::GetInst().FindSprite(_Name);

	if (nullptr == Sprite)
	{
		MSGASSERT("�ε����� ���� ��������Ʈ�� ����Ϸ��� �߽��ϴ�" + std::string(_Name));
		return;
	}

	CurIndex = _CurIndex;
}
