// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePowerUp.h"
#include "DynamoCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ABasePowerUp::ABasePowerUp() :
	VisualComponentRotationAngle(0.0f),
	VisualRotationRate(90.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComp->SetBoxExtent(FVector(25, 25, 25));
	CollisionComp->bGenerateOverlapEvents = true;

	VisualComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VisualComponent"));
	VisualComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABasePowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABasePowerUp::OnOverlapBegin);
}

// Called every frame
void ABasePowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the visual component
	VisualComponentRotationAngle += DeltaTime * VisualRotationRate;
	VisualComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 0, VisualComponentRotationAngle)));
}

void ABasePowerUp::PickedUpBy_Implementation(ADynamoCharacter* Character)
{
	Destroy();
}

void ABasePowerUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	ADynamoCharacter* character = Cast<ADynamoCharacter>(OtherActor);
	if (character)
	{
		PickedUpBy(character);
	}
}

float ABasePowerUp::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float actualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// If pick up gets damaged, destroy it.
	if (actualDamage > 0)
	{
		Destroy();
	}

	return actualDamage;
}

