//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
#include <iostream>
#include <string>

#pragma warning(disable:4996)

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class CCollision : public CKhuGleWin
{
public:
	CKhuGleLayer *m_pGameLayer;

	CKhuGleSprite *m_pCircles;
	CKhuGleSprite *m_pMyPlat;
	CKhuGleSprite* m_pSideLine[3];
	CKhuGleSprite* m_pBlockData[10];
	std::vector<CKhuGleSprite*> m_BlockManager;
	int m_Score = 0;
	float m_MoveSpeed = 1.5f;
	CCollision(int nW, int nH);
	void Update();
	void Init();
	std::pair<CKgVector2D,CKgVector2D> m_Pause;
	bool m_bPause = false;
	CKgPoint m_LButtonStart, m_LButtonEnd;
	int m_nLButtonStatus;
};

CCollision::CCollision(int nW, int nH) : CKhuGleWin(nW, nH) 
{
	m_nLButtonStatus = 0;
	m_Score = 0;
	m_Gravity = CKgVector2D(0., 0.);
	m_AirResistance = CKgVector2D(0., 0.);

	m_pScene = new CKhuGleScene(640, 480, KG_COLOR_24_RGB(100, 100, 150));

	m_pGameLayer = new CKhuGleLayer(600, 420, KG_COLOR_24_RGB(150, 150, 200), CKgPoint(20, 30));

	m_pScene->AddChild(m_pGameLayer);

	m_pCircles = new CKhuGleSprite(GP_STYPE_ELLIPSE, GP_CTYPE_DYNAMIC, CKgLine(CKgPoint(290, 210), CKgPoint(310, 230)),
		KG_COLOR_24_RGB(255, 0, 0), true, 100);
	m_pMyPlat = new CKhuGleSprite(GP_STYPE_BLOCK, GP_CTYPE_STATIC, CKgLine(CKgPoint(250, 400), CKgPoint(350, 400)),
		KG_COLOR_24_RGB(255, 0, 0), true, 10); // 받침

	m_pSideLine[0] = new CKhuGleSprite(GP_STYPE_LINE, GP_CTYPE_STATIC, CKgLine(CKgPoint(0, 0), CKgPoint(0, 420)),
		KG_COLOR_24_RGB(255, 0, 0), true, 10);
	m_pSideLine[1] = new CKhuGleSprite(GP_STYPE_LINE, GP_CTYPE_STATIC, CKgLine(CKgPoint(0, 0), CKgPoint(800, 0)),
		KG_COLOR_24_RGB(255, 0, 0), true, 10);
	m_pSideLine[2] = new CKhuGleSprite(GP_STYPE_LINE, GP_CTYPE_DYNAMIC, CKgLine(CKgPoint(600, 0), CKgPoint(600, 420)),
		KG_COLOR_24_RGB(255, 0, 0), true, 10);



	m_pGameLayer->AddChild(m_pCircles);

	m_pGameLayer->AddChild(m_pMyPlat);

	for (int i = 0; i < 3; i++)
	{
		m_pGameLayer->AddChild(m_pSideLine[i]);
	}
	for (int i = 0; i < 6; i++)// 블록 생성
	{
		m_pBlockData[i] = new CKhuGleSprite(GP_STYPE_BLOCK, GP_CTYPE_STATIC, CKgLine(CKgPoint(5 + i * 100, 50), CKgPoint(5 + (i + 1) * 100, 50)),
			KG_COLOR_24_RGB(255, 0, 0), true, 20, 3);
		m_pGameLayer->AddChild(m_pBlockData[i]);
		m_BlockManager.push_back(m_pBlockData[i]);
	}
	m_pCircles->m_Velocity = CKgVector2D(20, 210);

}
void CCollision::Init()
{
	if (m_pScene != nullptr)
	{
		m_pGameLayer->ClearChild();
		delete m_pGameLayer;
		delete m_pCircles;
		for (int i = 0; i < 3; i++)
		{
			delete m_pSideLine[i];
		}
		for (int i = 0; i < 6; i++)
		{
			delete m_pBlockData[i];
		}
		m_BlockManager.clear();
	}
	m_pGameLayer = new CKhuGleLayer(600, 420, KG_COLOR_24_RGB(150, 150, 200), CKgPoint(20, 30));

	m_pCircles = new CKhuGleSprite(GP_STYPE_ELLIPSE, GP_CTYPE_DYNAMIC, CKgLine(CKgPoint(290, 210), CKgPoint(310, 230)),
		KG_COLOR_24_RGB(255, 0, 0), true, 100);

	m_pMyPlat = new CKhuGleSprite(GP_STYPE_BLOCK, GP_CTYPE_STATIC, CKgLine(CKgPoint(250, 400), CKgPoint(350, 400)),
		KG_COLOR_24_RGB(255, 0, 0), true, 10); // 받침
	//사이트 라인
	m_pSideLine[0] = new CKhuGleSprite(GP_STYPE_LINE, GP_CTYPE_STATIC, CKgLine(CKgPoint(0, 0), CKgPoint(0, 420)),
		KG_COLOR_24_RGB(255, 0, 0), true, 10);
	m_pSideLine[1] = new CKhuGleSprite(GP_STYPE_LINE, GP_CTYPE_STATIC, CKgLine(CKgPoint(0, 0), CKgPoint(800, 0)),
		KG_COLOR_24_RGB(255, 0, 0), true, 10);
	m_pSideLine[2] = new CKhuGleSprite(GP_STYPE_LINE, GP_CTYPE_DYNAMIC, CKgLine(CKgPoint(600, 0), CKgPoint(600, 420)),
		KG_COLOR_24_RGB(255, 0, 0), true, 10);



	m_pGameLayer->AddChild(m_pCircles);
	m_pGameLayer->AddChild(m_pMyPlat);

	for (int i = 0; i < 3; i++)
	{
		m_pGameLayer->AddChild(m_pSideLine[i]);
	}
	for (int i = 0; i < 6; i++)// 블록 생성
	{
		m_pBlockData[i] = new CKhuGleSprite(GP_STYPE_BLOCK, GP_CTYPE_STATIC, CKgLine(CKgPoint(5 + i * 100, 50), CKgPoint(5 + (i + 1) * 100, 50)),
			KG_COLOR_24_RGB(255, 0, 0), true, 20, 3);
		m_pGameLayer->AddChild(m_pBlockData[i]);
		m_BlockManager.push_back(m_pBlockData[i]);
	}
	m_pCircles->m_Velocity = CKgVector2D(20, 210);
	m_Score = 0;

}
void CCollision::Update()
{
	if(m_bMousePressed[0]) {
		if(m_nLButtonStatus == 0)		{
			m_LButtonStart = CKgPoint(m_MousePosX, m_MousePosY);
		}
		m_LButtonEnd = CKgPoint(m_MousePosX, m_MousePosY);
		m_nLButtonStatus = 1;
	}
	else {
		if(m_nLButtonStatus == 1) {
			std::cout << m_LButtonStart.X << "," << m_LButtonStart.Y << std::endl;
			std::cout << m_LButtonEnd.X << "," << m_LButtonEnd.Y << std::endl;

			m_nLButtonStatus = 0;
		}
	}

	if(m_bKeyPressed['P'])  // 멈추고 시작하기 
	{
		if (m_bPause == false)
		{
			m_bPause = true;
			m_Pause.first = m_pCircles->m_Velocity;
			m_pCircles->m_Velocity = CKgVector2D(0., 0.);
			m_pMyPlat->m_nType = GP_STYPE_EMPTY;
			m_Pause.second = m_pMyPlat->m_Center;

		}
	}
	if (m_bKeyPressed['S'])
	{
		if (m_bPause == true)
		{
			m_bPause = false;
			m_pCircles->m_Velocity = m_Pause.first;
			m_pMyPlat->m_nType = GP_STYPE_BLOCK;
			m_pMyPlat->m_Center = m_Pause.second;
		}
	}

	// 서클 1의 이동 조정
//	if(m_bKeyPressed[VK_LEFT]) m_pCircle1->m_Velocity = CKgVector2D(-500, 0);
	if(m_bKeyPressed[VK_UP]) m_pCircles->m_Velocity = CKgVector2D(0, -500);
//	if(m_bKeyPressed[VK_RIGHT]) m_pCircle1->m_Velocity = CKgVector2D(500, 0);
	if(m_bKeyPressed[VK_DOWN]) m_pCircles->m_Velocity = CKgVector2D(0, 500);

	if (m_bKeyPressed[VK_RIGHT]) // 받침대 이동
	{
		m_pMyPlat->MoveTo(m_pMyPlat->m_Center.x + m_MoveSpeed, m_pMyPlat->m_Center.y);
		if (m_pMyPlat->m_Center.x > 540) //나가지 못하게 막기
			m_pMyPlat->m_Center.x = 540;
	}
	if (m_bKeyPressed[VK_LEFT]) // 받침대 이동
	{
		m_pMyPlat->MoveTo(m_pMyPlat->m_Center.x - m_MoveSpeed, m_pMyPlat->m_Center.y);
		if (m_pMyPlat->m_Center.x < 60) //나가지 못하게 막기
			m_pMyPlat->m_Center.x = 60;
	}
	for(auto &Layer : m_pScene->m_Children)
	{
		for(auto &Sprite : Layer->m_Children)
		{
			CKhuGleSprite *Ball = (CKhuGleSprite *)Sprite;

			Ball->m_bCollided = false;

			if(Ball->m_nType == GP_STYPE_RECT) continue;

			if(Ball->m_nType != GP_STYPE_ELLIPSE) continue;
			if(Ball->m_nCollisionType != GP_CTYPE_DYNAMIC) continue;

			Ball->m_Acceleration.x = m_Gravity.x - Ball->m_Velocity.x * m_AirResistance.x;			
			Ball->m_Acceleration.y = m_Gravity.y - Ball->m_Velocity.y * m_AirResistance.y;

			Ball->m_Velocity.x += Ball->m_Acceleration.x * m_ElapsedTime;
			Ball->m_Velocity.y += Ball->m_Acceleration.y * m_ElapsedTime;

			Ball->MoveBy(Ball->m_Velocity.x*m_ElapsedTime, Ball->m_Velocity.y*m_ElapsedTime);

			if (Ball->m_Center.x < 0) Ball->m_nType = GP_STYPE_EMPTY;//Ball->MoveTo(m_nW+Ball->m_Center.x, Ball->m_Center.y); //공이 밖으로 나가는지 처리하는 부분
			if(Ball->m_Center.x > m_nW) Ball->m_nType = GP_STYPE_EMPTY;//Ball->MoveTo(Ball->m_Center.x-m_nW, Ball->m_Center.y);
			if(Ball->m_Center.y < 0) Ball->m_nType = GP_STYPE_EMPTY;//Ball->MoveTo(Ball->m_Center.x, m_nH+Ball->m_Center.y);
			if(Ball->m_Center.y > m_nH) Ball->m_nType = GP_STYPE_EMPTY;//Ball->MoveTo(Ball->m_Center.x, Ball->m_Center.y-m_nH);

			if(CKgVector2D::abs(Ball->m_Velocity) < 0.01)
				Ball->m_Velocity = CKgVector2D(0, 0);
		}

		std::vector<std::pair<CKhuGleSprite*, CKhuGleSprite*>> CollisionPairs;
		std::vector<CKhuGleSprite*> VirtualBalls; // 선충돌에서 가상의 공을 담아둠
		
		for(auto &SpriteA : Layer->m_Children) //충돌 처리 부분 센터간 거리가 반지름 합보다 작아질때 // 겹치면 겹친 만큼 밀어줌
		{
			CKhuGleSprite *Ball = (CKhuGleSprite *)SpriteA;
			if(Ball->m_nType != GP_STYPE_ELLIPSE) continue;

			for(auto &SpriteB : Layer->m_Children)
			{
				CKhuGleSprite *Target = (CKhuGleSprite *)SpriteB;

				if(Ball == Target) continue;

				if(((CKhuGleSprite *)Target)->m_nType == GP_STYPE_ELLIPSE) 
				{
					CKgVector2D PosVec = Ball->m_Center - Target->m_Center; //센터간 거리 타겟에서 볼로 향하는 벡터
					double Overlapped = CKgVector2D::abs(PosVec) - Ball->m_Radius - Target->m_Radius; //0보다 작으면 충돌
					if(Overlapped <= 0) // 충돌 확인
					{
						CollisionPairs.push_back({Ball, Target});

						if(CKgVector2D::abs(PosVec) == 0) // 중심이 완전히 겹치면 랜덤값으로 이동
						{
							if(Ball->m_nCollisionType != GP_CTYPE_STATIC)
								Ball->MoveBy(rand()%3-1, rand()%3-1);
							if(Target->m_nCollisionType != GP_CTYPE_STATIC)
								Target->MoveBy(rand()%3-1, rand()%3-1);
						}
						else
						{
							if(Ball->m_nCollisionType != GP_CTYPE_STATIC)
							{
								if(Target->m_nCollisionType == GP_CTYPE_STATIC)
									Ball->MoveBy(-PosVec.x*Overlapped/CKgVector2D::abs(PosVec), -PosVec.y*Overlapped/CKgVector2D::abs(PosVec));
								else
									Ball->MoveBy(-PosVec.x*Overlapped/CKgVector2D::abs(PosVec)*0.5, -PosVec.y*Overlapped/CKgVector2D::abs(PosVec)*0.5);
							}
							if(Target->m_nCollisionType != GP_CTYPE_STATIC) // 타겟과 볼이 스태틱이냐 다이나믹이냐에 따라 처리가 다름
							{
								if(Ball->m_nCollisionType == GP_CTYPE_STATIC)
									Target->MoveBy(PosVec.x*Overlapped/CKgVector2D::abs(PosVec), PosVec.y*Overlapped/CKgVector2D::abs(PosVec));
								else
									Target->MoveBy(PosVec.x*Overlapped/CKgVector2D::abs(PosVec)*0.5, PosVec.y*Overlapped/CKgVector2D::abs(PosVec)*0.5);
							}
						}

						Ball->m_bCollided = true;
						Target->m_bCollided = true;
					}
				}
				/// ######################################HOMEWORK1######################################################	
				if (Target->m_nType == GP_STYPE_LINE || Target->m_nType == GP_STYPE_BLOCK)
				{
					CKgVector2D LinePos = CKgVector2D(Target->m_lnLine.End.X, Target->m_lnLine.End.Y)
						- CKgVector2D(Target->m_lnLine.Start.X, Target->m_lnLine.Start.Y); // 라인의 시작점에서 끝점으로 가는 벡터

					CKgVector2D LineCirclePos = Ball->m_Center - CKgVector2D(Target->m_lnLine.Start.X, Target->m_lnLine.Start.Y);// 라인 시작에서 원중신으로 가는 벡터

					double AA = LinePos.Dot(LinePos); // 라인 벡터의 크기 제곱

					double ProjectionRate = max(0., min(AA, LinePos.Dot(LineCirclePos))) / AA; //  라인을 끝에서 넘어간 공들에 대한 
					//제한 처리와 라인 벡터와 공과 선벡터에서 나누어주기 프로젝션비율

					CKgVector2D ProjectionPoint = CKgVector2D(Target->m_lnLine.Start) + ProjectionRate * LinePos; // 최종적으로 원점까지 고려한 프로젝션 포인트

					CKgVector2D Normal = Ball->m_Center - ProjectionPoint; // 프로젝션 포인트에서 공의 센터를 뺀 라인에서의 노말 벡터

					double Overlapped = CKgVector2D::abs(Normal) - Ball->m_Radius - Target->m_nWidth / 2; // 라인과 공과 겹치는 부분

					if (Overlapped <= 0)
					{

						CKhuGleSprite* VirtualBall = new CKhuGleSprite(GP_STYPE_ELLIPSE, GP_CTYPE_STATIC, CKgLine((int)(ProjectionPoint.x - Target->m_nWidth / 2.)
							, (int)(ProjectionPoint.y + Target->m_nWidth / 2.), (int)(ProjectionPoint.x + Target->m_nWidth / 2.), (int)(ProjectionPoint.y + Target->m_nWidth / 2.)),
							KG_COLOR_24_RGB(255, 0, 0), false, 100); // 프로젝션 위치에 가상의 공을 만들어준다

						VirtualBall->m_Mass = 1E50; // 가상의 공의 무게는 엄청크게
						VirtualBalls.push_back(VirtualBall);

						CollisionPairs.push_back({ Ball,VirtualBall });

						if (CKgVector2D::abs(Normal) == 0) // 중심이 완전히 겹치면 랜덤값으로 이동
						{
							if (Ball->m_nCollisionType != GP_CTYPE_STATIC) //공이 스태틱이 아니면 겹칠시 무작위 이동
								Ball->MoveBy(rand() % 3 - 1, rand() % 3 - 1);
						}
						else
						{
							if (Ball->m_nCollisionType != GP_CTYPE_STATIC)
								Ball->MoveBy(-Normal.x * Overlapped / CKgVector2D::abs(Normal), -Normal.y * Overlapped / CKgVector2D::abs(Normal));
						}
						Ball->m_bCollided = true; // 충돌 처리
						Target->m_bCollided = true;
						
						Target->m_nLife--; // 블록라이프 줄여주기
						if (Target->m_nLife <= 0) // 라이프가 0 이되면 EMPTY 타입으로 교체
						{
							Target->m_nType = GP_STYPE_EMPTY;
							m_Score += 100;
						}
						Target->AdjustColor(); // 충돌 시 컬러 조정
					}
				}
			/// ######################################HOMEWORK1######################################################
			}
		}
		for(auto &Pair : CollisionPairs) // collisionPair는 초기화 안해줘도 되나?
		{
			CKhuGleSprite *BallA = Pair.first;
			CKhuGleSprite *BallB = Pair.second;

			CKgVector2D PosVec = BallB->m_Center - BallA->m_Center;
			double Distance = CKgVector2D::abs(PosVec);
			if(Distance == 0) Distance = 1E-6;
			CKgVector2D Normal = (1./Distance)*PosVec;

			double kx = (BallA->m_Velocity.x - BallB->m_Velocity.x);
			double ky = (BallA->m_Velocity.y - BallB->m_Velocity.y);
			double p = 2.0 * (Normal.x * kx + Normal.y * ky) / (BallA->m_Mass + BallB->m_Mass);

			BallA->m_Velocity.x = BallA->m_Velocity.x - p * BallB->m_Mass * Normal.x;
			BallA->m_Velocity.y = BallA->m_Velocity.y - p * BallB->m_Mass * Normal.y;

			BallB->m_Velocity.x = BallB->m_Velocity.x + p * BallA->m_Mass * Normal.x;
			BallB->m_Velocity.y = BallB->m_Velocity.y + p * BallA->m_Mass * Normal.y;
		}
		VirtualBalls.clear();
	}

	m_pScene->Render();
	DrawSceneTextPos("OldSchool Arkanoid", CKgPoint(0, 0));
	DrawSceneTextPos("Score : ", CKgPoint(500, 0));
	DrawSceneTextPos(std::to_string(m_Score).c_str(), CKgPoint(600, 0)); // 스코어
	if (m_pCircles->m_nType == GP_STYPE_EMPTY)
	{
		DrawSceneTextPos("You Died", CKgPoint(280, 200));
		if (m_bKeyPressed['R'])
		{
			Init();
		}
	}
	bool b_clear = true;
	for (auto block : m_BlockManager)
	{
		if (block->m_nType == GP_STYPE_BLOCK)
		{
			b_clear = false;
			break;
		}
	}
	if (b_clear == true)
	{
		m_pCircles->m_Velocity = CKgVector2D(0, 0);
		DrawSceneTextPos("Stage Clear!!!", CKgPoint(260, 200));
		if (m_bKeyPressed['R'])
		{
			Init();
		}
	}
	if(m_bPause == true)
		DrawSceneTextPos("Pause", CKgPoint(280, 200));
	CKhuGleWin::Update();
}

int main()
{

	CCollision *pCollision = new CCollision(640, 480);

	KhuGleWinInit(pCollision);

	return 0;
}