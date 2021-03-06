// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Net/Manager.h"
#include "GameNet/GameNetMrg.h"
#include "CarsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CARS_API ACarsGameModeBase : public AGameModeBase
{
  GENERATED_BODY()
public:
  ACarsGameModeBase(const class FObjectInitializer& ObjectInitializer);
  virtual ~ACarsGameModeBase();
  virtual void Destroyed() override;
  virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

  virtual void Tick(float DeltaSeconds) override;

  const CGameNetMrg& GetGameNetMgr() const { return m_oGameNetMrg; }

  /** Remove the current menu widget and create a new one from the specified class, if provided. */
  UFUNCTION(BlueprintCallable, Category = CarsNet)
  void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
  UFUNCTION(BlueprintCallable, Category = CarsNet)
  void OnServerButtonClick(FString sPort);
  UFUNCTION(BlueprintCallable, Category = CarsNet)
  void OnClientButtonClick(FString sIP, FString sPort);
  UFUNCTION(BlueprintCallable, Category = CarsNet)
  void OnServerStartButtonClick();

protected:
  /** Called when the game starts. */
  virtual void BeginPlay() override;

  /** Called when the game ends. */
  virtual void EndPlay(EEndPlayReason::Type eEndPlayReason) override;

  /** The widget class we will use as our menu when the game starts. */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CarsNet)
  TSubclassOf<UUserWidget> StartingWidgetClass;

  /** The widget instance that we are using as our menu. */
  UPROPERTY()
  UUserWidget* CurrentWidget;
  //
  CGameNetMrg m_oGameNetMrg;
  Net::CManager* m_pManager = nullptr;
};
