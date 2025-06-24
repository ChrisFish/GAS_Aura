// Copyright Downwind Cloud Software, LLC


#include "Character/AuraEnemy.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(ENEMY_HIGHLIGHT_STENCIL_VALUE_RED);
}

void AAuraEnemy::UnhightlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}
