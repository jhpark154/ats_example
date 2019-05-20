#include "stdafx.h"
#include "Plugin.h"

// 전역 변수
int g_iReverser = 1;
int g_iPowerNotch = 0;
int g_iBrakeNotch = 0;

ATS_VEHICLESPEC g_VehicleSpec; // 열차 제원
ATS_VEHICLESTATE g_PreviousState; // 열차 이전 상태

struct DSDSystem
{
	bool bkey; // DSD 키가 입력 되었는지
	bool bBraking; // DSD가 발동 되었는지
	int iTime; // DSD 키가 눌려있지 않은 시간
};
DSDSystem g_DSDSystem = { false, false, 0 }; // DSD 시스템

// 플러그인이 로드되었습니다.
#define _CRT_SECURE_NO_WARNINGS
ATS_API void WINAPI Load()
{}

// 플러그인이 언로드되었습니다.
ATS_API void WINAPI Dispose()
{
	printf("Dispose\n");
}

// ATS 플러그인 버전을 리턴합니다.
ATS_API int WINAPI GetPluginVersion()
{
	return ATS_VERSION;
}

// 열차가 로드되었습니다.
ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC VehicleSpec)
{
	printf("SetVehicleSpec\n");
	g_VehicleSpec = VehicleSpec; // 열차 제원 저장
}

// 게임이 시작되었습니다.
ATS_API void WINAPI Initialize(int iHandle)
{
	printf("Initialize : %d\n", iHandle);
}

// 매 프레임마다 실행됩니다.
ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE VehicleState, int *pPanel, int *pSound)
{
	// 놋치를 설정함
	ATS_HANDLES Output;
	Output.Reverser = g_iReverser; // 역전기
	Output.Power = g_iPowerNotch; // 가속 놋치
	Output.Brake = g_iBrakeNotch; // 브레이크 놋치

	if (g_DSDSystem.bBraking) // DSD가 발동됨
	{
		if (VehicleState.Speed >= 0.0f && g_iPowerNotch >= 0 && g_iBrakeNotch >= 4) // 기관사가 4단 이상의 제동을 걸음
		{
			g_DSDSystem.bBraking = false; // DSD 해제
		}
		Output.Power = 0;
		Output.Brake = g_VehicleSpec.BrakeNotches; // 최대 브레이크 단 수
	}
	else if (VehicleState.Speed >= 5.0f && g_iBrakeNotch == 0) // 5km/h 이상, 브레이크 0단 일 경우(= 열차가 주행 중 일때)
	{
		if (g_DSDSystem.bkey == false) // DSD 키가 눌려 있지 않음
		{
			g_DSDSystem.iTime += (VehicleState.Time - g_PreviousState.Time); // 시간 누적
			if (g_DSDSystem.iTime >= 3000) // 3초 이상 지남
			{
				g_DSDSystem.bBraking = true; // DSD 발동
				g_DSDSystem.iTime = 0;
			}
		}
		else // DSD 키가 눌려 있음
		{
			g_DSDSystem.iTime = 0; // 시간 초기화
		}
	}

	g_PreviousState = VehicleState; // 열차 상태 저장
    return Output;
}

// 가속 놋치가 움직였습니다.
ATS_API void WINAPI SetPower(int iNotch)
{
	printf("SetPower : %d\n", iNotch);
	g_iPowerNotch = iNotch;
}

// 브레이크 놋치가 움직였습니다.
ATS_API void WINAPI SetBrake(int iNotch)
{
	printf("SetBrake : %d\n", iNotch);
	g_iBrakeNotch = iNotch;
}

// 역전기가 움직였습니다.
ATS_API void WINAPI SetReverser(int iReverser)
{
	printf("SetReverser : %d\n", iReverser);
	g_iReverser = iReverser;
}

// 플러그인 용 키가 눌렸습니다.
ATS_API void WINAPI KeyDown(int iKeyCode)
{
	printf("KeyDown : %d\n", iKeyCode);
	if (iKeyCode == ATS_KEY_D)
	{
		g_DSDSystem.bkey = true; // DSD 키가 눌렸음
	}
}

// 플러그인 용 키가 눌렸다가 떼어졌습니다.
ATS_API void WINAPI KeyUp(int iKeyCode)
{
	printf("KeyUp : %d\n", iKeyCode);
	if (iKeyCode == ATS_KEY_D)
	{
		g_DSDSystem.bkey = false; // DSD 키가 떼어짐
	}
}

// 경적이 울렸습니다.
ATS_API void WINAPI HornBlow(int iHorn)
{
	printf("HornBlow : %d\n", iHorn);
}

// 출입문이 열렸습니다.
ATS_API void WINAPI DoorOpen()
{
	printf("DoorOpen\n");
}

// 출입문이 닫혔습니다.
ATS_API void WINAPI DoorClose()
{
	printf("DoorClose\n");
}

// 신호 값이 바뀌었습니다.
ATS_API void WINAPI SetSignal(int iSignal)
{
	printf("SetSignal : %d\n", iSignal);
}

// Beacon을 통과했습니다.
ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA Beacon)
{
	printf("SetBeaconData : %d, %d, %.2f, %d\n", Beacon.Type, Beacon.Signal, Beacon.Distance, Beacon.Optional);
}
