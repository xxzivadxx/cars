#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CheckPoint.generated.h"

class AFinishLine;

UCLASS()
class CARS_API ACheckPoint : public ATriggerBox
{
  GENERATED_BODY()

protected:

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:

  // constructor sets default values for this actor's properties
  ACheckPoint();


  // overlap begin function
  UFUNCTION()
    void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

  // overlap end function
  UFUNCTION()
    void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

  inline void SetFinishLine(AFinishLine* _pFinishLine) { m_pFinishLine = _pFinishLine; }

private:
  AFinishLine * m_pFinishLine;
};