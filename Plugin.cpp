#include "stdafx.h"
#include "Plugin.h"

// ���� ����
int g_iReverser = 1;
int g_iPowerNotch = 0;
int g_iBrakeNotch = 0;

ATS_VEHICLESPEC g_VehicleSpec; // ���� ����
ATS_VEHICLESTATE g_PreviousState; // ���� ���� ����

struct DSDSystem
{
	bool bkey; // DSD Ű�� �Է� �Ǿ�����
	bool bBraking; // DSD�� �ߵ� �Ǿ�����
	int iTime; // DSD Ű�� �������� ���� �ð�
};
DSDSystem g_DSDSystem = { false, false, 0 }; // DSD �ý���

// �÷������� �ε�Ǿ����ϴ�.
#define _CRT_SECURE_NO_WARNINGS
ATS_API void WINAPI Load()
{}

// �÷������� ��ε�Ǿ����ϴ�.
ATS_API void WINAPI Dispose()
{
	printf("Dispose\n");
}

// ATS �÷����� ������ �����մϴ�.
ATS_API int WINAPI GetPluginVersion()
{
	return ATS_VERSION;
}

// ������ �ε�Ǿ����ϴ�.
ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC VehicleSpec)
{
	printf("SetVehicleSpec\n");
	g_VehicleSpec = VehicleSpec; // ���� ���� ����
}

// ������ ���۵Ǿ����ϴ�.
ATS_API void WINAPI Initialize(int iHandle)
{
	printf("Initialize : %d\n", iHandle);
}

// �� �����Ӹ��� ����˴ϴ�.
ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE VehicleState, int *pPanel, int *pSound)
{
	// ��ġ�� ������
	ATS_HANDLES Output;
	Output.Reverser = g_iReverser; // ������
	Output.Power = g_iPowerNotch; // ���� ��ġ
	Output.Brake = g_iBrakeNotch; // �극��ũ ��ġ

	if (g_DSDSystem.bBraking) // DSD�� �ߵ���
	{
		if (VehicleState.Speed >= 0.0f && g_iPowerNotch >= 0 && g_iBrakeNotch >= 4) // ����簡 4�� �̻��� ������ ����
		{
			g_DSDSystem.bBraking = false; // DSD ����
		}
		Output.Power = 0;
		Output.Brake = g_VehicleSpec.BrakeNotches; // �ִ� �극��ũ �� ��
	}
	else if (VehicleState.Speed >= 5.0f && g_iBrakeNotch == 0) // 5km/h �̻�, �극��ũ 0�� �� ���(= ������ ���� �� �϶�)
	{
		if (g_DSDSystem.bkey == false) // DSD Ű�� ���� ���� ����
		{
			g_DSDSystem.iTime += (VehicleState.Time - g_PreviousState.Time); // �ð� ����
			if (g_DSDSystem.iTime >= 3000) // 3�� �̻� ����
			{
				g_DSDSystem.bBraking = true; // DSD �ߵ�
				g_DSDSystem.iTime = 0;
			}
		}
		else // DSD Ű�� ���� ����
		{
			g_DSDSystem.iTime = 0; // �ð� �ʱ�ȭ
		}
	}

	g_PreviousState = VehicleState; // ���� ���� ����
    return Output;
}

// ���� ��ġ�� ���������ϴ�.
ATS_API void WINAPI SetPower(int iNotch)
{
	printf("SetPower : %d\n", iNotch);
	g_iPowerNotch = iNotch;
}

// �극��ũ ��ġ�� ���������ϴ�.
ATS_API void WINAPI SetBrake(int iNotch)
{
	printf("SetBrake : %d\n", iNotch);
	g_iBrakeNotch = iNotch;
}

// �����Ⱑ ���������ϴ�.
ATS_API void WINAPI SetReverser(int iReverser)
{
	printf("SetReverser : %d\n", iReverser);
	g_iReverser = iReverser;
}

// �÷����� �� Ű�� ���Ƚ��ϴ�.
ATS_API void WINAPI KeyDown(int iKeyCode)
{
	printf("KeyDown : %d\n", iKeyCode);
	if (iKeyCode == ATS_KEY_D)
	{
		g_DSDSystem.bkey = true; // DSD Ű�� ������
	}
}

// �÷����� �� Ű�� ���ȴٰ� ���������ϴ�.
ATS_API void WINAPI KeyUp(int iKeyCode)
{
	printf("KeyUp : %d\n", iKeyCode);
	if (iKeyCode == ATS_KEY_D)
	{
		g_DSDSystem.bkey = false; // DSD Ű�� ������
	}
}

// ������ ��Ƚ��ϴ�.
ATS_API void WINAPI HornBlow(int iHorn)
{
	printf("HornBlow : %d\n", iHorn);
}

// ���Թ��� ���Ƚ��ϴ�.
ATS_API void WINAPI DoorOpen()
{
	printf("DoorOpen\n");
}

// ���Թ��� �������ϴ�.
ATS_API void WINAPI DoorClose()
{
	printf("DoorClose\n");
}

// ��ȣ ���� �ٲ�����ϴ�.
ATS_API void WINAPI SetSignal(int iSignal)
{
	printf("SetSignal : %d\n", iSignal);
}

// Beacon�� ����߽��ϴ�.
ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA Beacon)
{
	printf("SetBeaconData : %d, %d, %.2f, %d\n", Beacon.Type, Beacon.Signal, Beacon.Distance, Beacon.Optional);
}
