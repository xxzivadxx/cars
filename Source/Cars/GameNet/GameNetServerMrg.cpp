// Fill out your copyright notice in the Description page of Project Settings.

#include "GameNetServerMrg.h"
#include "GameNet/GameBuffer.h"
#include "Net/paquete.h"
#include "Kismet/GameplayStatics.h"
#include "CarsGameModeBase.h"
#include "Engine/World.h"
#include "Engine/LevelStreaming.h"
#include "Game/Car.h"

CGameNetServerMrg::CGameNetServerMrg() : m_pController(nullptr)
{
  m_pManager = Net::CManager::getSingletonPtr();
}

CGameNetServerMrg::CGameNetServerMrg(ACarsGameModeBase* _pController) : m_pController(_pController)
{
  m_pManager = Net::CManager::getSingletonPtr();
}

void CGameNetServerMrg::dataPacketReceived(Net::CPaquete* packet)
{
  // Creamos un buffer con los datos para leer de manera m�s c�moda
  CGameBuffer data;
  data.write(packet->getData(), packet->getDataLength());
  data.reset();
  NetMessageType eMessageType;
  data.read(eMessageType);
  switch (eMessageType)
  {
  case NetMessageType::LOAD_MAP:
  {
    char sMap[128];
    data.read(sMap);
    GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, *FString(sMap));
    UGameplayStatics::OpenLevel(m_pController->GetWorld(), sMap);
    NetMessageType iID = NetMessageType::MAP_LOADED;
    m_pManager->send(&iID, sizeof(iID));
    //ULevelStreaming* level = UGameplayStatics::GetStreamingLevel(m_pController->GetWorld(), sMap);
    //ACar* pCar;
    //level->OnLevelLoaded.AddDynamic(this, &AClass::OnLevelStreamLoaded);
  } break;
  case NetMessageType::MAP_LOADED:
  {
    for (auto client : m_pManager->getConnections())
    {
      CGameBuffer data;
      NetMessageType iID = NetMessageType::LOAD_PLAYER;
      data.write(iID);
      data.write(client.first);
      m_pManager->send(data.getbuffer(), data.getSize());
    }
  } break;
  case NetMessageType::LOAD_PLAYER:
  {
    unsigned int uClient;
    data.read(uClient);
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Name = FName("Car", uClient);
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (m_pController->GetWorld())
    {
      TSubclassOf<ACar> ACarPawnClass;
      ACar* pCar = m_pController->GetWorld()->SpawnActor<ACar>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

      if (pCar)
      {
        if (uClient == m_pManager->getID())
        {
          APlayerController* OurPlayerController = GEngine->GetFirstLocalPlayerController(m_pController->GetWorld());
          if (OurPlayerController)
          {
            OurPlayerController->SetPawn(pCar);
          }
          pCar->AutoPossessPlayer = EAutoReceiveInput::Player0;
        }
      }
    }
  } break;
  default:
    break;
  }
}

//--------------------------------

void CGameNetServerMrg::connexionPacketReceived(Net::CPaquete* packet)
{
}

//--------------------------------

void CGameNetServerMrg::disconnexionPacketReceived(Net::CPaquete* packet)
{

}

//--------------------------------

void CGameNetServerMrg::Tick()
{
  m_pManager->tick();
}