//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "CPlayer.h"

//-----------------------------------------------------------------------------
// Name : CPlayer () (Constructor)
// Desc : CPlayer Class Constructor
//-----------------------------------------------------------------------------
CPlayer::CPlayer(const BackBuffer *pBackBuffer)
{
	//m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	m_pSprite = new Sprite("data/spaceship.bmp", RGB(0xff,0x00, 0xff));
	m_pSprite->setBackBuffer( pBackBuffer );
	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;

	// Animation frame crop rectangle
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 128;
	r.bottom = 128;

	m_pExplosionSprite	= new AnimatedSprite("data/explosion.bmp", "data/explosionmask.bmp", r, 15);
	m_pExplosionSprite->setBackBuffer( pBackBuffer );
	m_bExplosion		= false;
	m_iExplosionFrame	= 0;

	m_pHeart1 = new Sprite("data/heart.bmp", RGB(0xff, 0x00, 0xff));
	m_pHeart1->setBackBuffer(pBackBuffer);
	m_pHeart2 = new Sprite("data/heart.bmp", RGB(0xff, 0x00, 0xff));
	m_pHeart2->setBackBuffer(pBackBuffer);
	m_pHeart3 = new Sprite("data/heart.bmp", RGB(0xff, 0x00, 0xff));
	m_pHeart3->setBackBuffer(pBackBuffer);

	m_bBullet = false;
	for (int i = 0; i < 4; i++)
	{
		m_pCrates.push_back(new Sprite("data/bigasteroid.bmp", RGB(0xff, 0x00, 0xff)));
		m_pCrates.push_back(new Sprite("data/mediumasteroid.bmp", RGB(0xff, 0x00, 0xff)));
	}


	for (int i = 0; i < m_pCrates.size(); i++)
		m_pCrates[i]->setBackBuffer(pBackBuffer);

	for(int i=0;i<3;i++)
	    m_pBullets.push_back(new Sprite("data/bulletimgandmask.bmp", RGB(0xff, 0x00, 0xff)));
	//m_pBullets.push_back(new Sprite("data/bulletimgandmask.bmp", RGB(0xff, 0x00, 0xff)));
	for (int i = 0; i < m_pBullets.size(); i++)
	    m_pBullets[i]->setBackBuffer(pBackBuffer);
	m_bEnemyBullets = false;
	m_bEnemies = false;
	for (int i = 0; i < 2; i++) {
		m_pEnemySprites.push_back(new Sprite("data/planeimgandmaskdown.bmp", RGB(0xff, 0x00, 0xff)));
		m_pEnemyBullets.push_back(new Sprite("data/bulletimgandmask.bmp", RGB(0xff, 0x00, 0xff)));
		m_pEnemySprites[i]->setBackBuffer(pBackBuffer);
		m_pEnemyBullets[i]->setBackBuffer(pBackBuffer);
	}

	for (int i = 0; i < 2; i++) {
		m_pLives.push_back(new Sprite("data/live.bmp", RGB(0xff, 0x00, 0xff)));
		m_pLives[i]->setBackBuffer(pBackBuffer);
	}

	


}

//-----------------------------------------------------------------------------
// Name : ~CPlayer () (Destructor)
// Desc : CPlayer Class Destructor
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	delete m_pSprite;
	delete m_pExplosionSprite;
	delete m_pHeart1;
	delete m_pHeart2;
	delete m_pHeart3;
	
	for (int i = 0; i < m_pBullets.size(); i++)
		delete m_pBullets[i];
	
	for (int i = 0; i < m_pCrates.size(); i++)
		delete m_pCrates[i];

	for (int i = 0; i < m_pEnemySprites.size(); i++)
	{
		delete m_pEnemySprites[i];
		delete m_pEnemyBullets[i];
	}

	for (int i = 0; i < m_pLives.size(); i++)
		delete m_pLives[i];
	
}

void CPlayer::Update(float dt)
{
	// Update sprite
	m_pSprite->update(dt);

	// Get velocity
	double v = m_pSprite->mVelocity.Magnitude();

	// NOTE: for each async sound played Windows creates a thread for you
	// but only one, so you cannot play multiple sounds at once.
	// This creation/destruction of threads also leads to bad performance
	// so this method is not recommanded to be used in complex projects.

	// update internal time counter used in sound handling (not to overlap sounds)
	m_fTimer += dt;

	// A FSM is used for sound manager 
	switch(m_eSpeedState)
	{
	case SPEED_STOP:
		if(v > 35.0f)
		{
			m_eSpeedState = SPEED_START;
			PlaySound("data/jet-start.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		break;
	case SPEED_START:
		if(v < 25.0f)
		{
			m_eSpeedState = SPEED_STOP;
			PlaySound("data/jet-stop.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		else
			if(m_fTimer > 1.f)
			{
				PlaySound("data/jet-cabin.wav", NULL, SND_FILENAME | SND_ASYNC);
				m_fTimer = 0;
			}
		break;
	}

	// NOTE: For sound you also can use MIDI but it's Win32 API it is a bit hard
	// see msdn reference: http://msdn.microsoft.com/en-us/library/ms711640.aspx
	// In this case you can use a C++ wrapper for it. See the following article:
	// http://www.codeproject.com/KB/audio-video/midiwrapper.aspx (with code also)
}

void CPlayer::Draw()
{
	if (!m_bExplosion)
		m_pSprite->draw();
	else
		m_pExplosionSprite->draw();

	for (int i = 0; i < m_pCrates.size(); i++)
		m_pCrates[i]->draw();

	for (int i = 0; i < m_pLives.size(); i++)
		m_pLives[i]->draw();

	if (m_bBullet) {

		m_pBullets[0]->draw();
	}
	
	if (m_bEnemies == true) {
		for (int i = 0; i < m_pEnemySprites.size(); i++)
			m_pEnemySprites[i]->draw();
	}
	if (m_bEnemyBullets == true)
		for (int i = 0; i < m_pEnemySprites.size(); i++)
			m_pEnemyBullets[i]->draw();

	m_pHeart1->draw();
	m_pHeart2->draw();
	m_pHeart3->draw();
}

void CPlayer::Shoot()
{
		m_pBullets[0]->mPosition.x = m_pSprite->mPosition.x;
		m_pBullets[0]->mPosition.y = m_pSprite->mPosition.y - m_pSprite->height() / 2;
		/*if(bullets!=0)
		   m_pBullets[bullets]->draw();
		//bullets--;*/
	     this->m_bBullet = true;

}

void CPlayer::Move(ULONG ulDirection)
{
	if (m_pSprite->mPosition.x < m_pSprite->width() / 2) {
		m_pSprite->mVelocity.x = 0;
		m_pSprite->mPosition.x++;
	}
	
	if (m_pSprite->mPosition.x > GetSystemMetrics(SM_CXSCREEN) - m_pSprite->width() / 2) {
		m_pSprite->mVelocity.x = 0;
		m_pSprite->mPosition.x--;
	}
	if (m_pSprite->mPosition.y < m_pSprite->height() / 2) {
		m_pSprite->mVelocity.y = 0;
		m_pSprite->mPosition.y++;
	}
		
	
	if (m_pSprite->mPosition.y > GetSystemMetrics(SM_CYSCREEN) - m_pSprite->height()) {
		m_pSprite->mVelocity.y = 0;
		m_pSprite->mPosition.y--;
	}
	

	if(m_bBullet)
		m_pBullets[0]->mPosition.y -= 3;

	for (int i = 0; i < m_pCrates.size(); i++) {
		m_pCrates[i]->mPosition.y += 2.5;
		if (m_pCrates[i]->mPosition.y > GetSystemMetrics(SM_CYSCREEN) - m_pCrates[i]->height()) {
			srand(time(NULL));

			m_pCrates[i]->mPosition.y= -rand()%500-1;
			m_pCrates[i]->mPosition.x = rand() % 1450+1;
		}
	}



	for (int i = 0; i < m_pLives.size(); i++) {
		m_pLives[i]->mPosition.y += 2.7;
		if (m_pLives[i]->mPosition.y > GetSystemMetrics(SM_CYSCREEN) - m_pLives[i]->height()) {
			srand(time(NULL));
			m_pLives[i]->mPosition.y = -rand() % 500 - 1;
			m_pLives[i]->mPosition.x = rand() % 1450 + 50;
		}

	}
	


	if (m_bEnemies==true) {
		for (int i = 0; i < m_pEnemySprites.size(); i++) {
			if (m_pEnemySprites[i]->mPosition.y <= 200) {
				m_pEnemySprites[i]->mPosition.y += 1.5;

				m_pEnemyBullets[i]->mPosition.x = m_pEnemySprites[i]->mPosition.x;
				m_pEnemyBullets[i]->mPosition.y = m_pEnemySprites[i]->mPosition.y + m_pEnemySprites[i]->height() / 2;
			}
			else {
				m_bEnemyBullets = true;
				m_pEnemyBullets[i]->mPosition.y += 2.5;
				if (m_pEnemyBullets[i]->mPosition.y > GetSystemMetrics(SM_CYSCREEN) - m_pEnemyBullets[i]->height()/2) {

					m_pEnemyBullets[i]->mPosition.x = m_pEnemySprites[i]->mPosition.x;
					m_pEnemyBullets[i]->mPosition.y = m_pEnemySprites[i]->mPosition.y + m_pEnemySprites[i]->height() / 2;
				}
				
				if (i % 2 == 0) {
					if (changedirection1==false) {
						
						m_pEnemySprites[i]->mPosition.x += 1.2;
						if (m_pEnemySprites[i]->mPosition.x >= 700) {
							changedirection1 = true;
						}
					}
					else if (changedirection1==true) {
						m_pEnemySprites[i]->mPosition.x -= 1.2;
						if (m_pEnemySprites[i]->mPosition.x <= 100)
							changedirection1 = false;
					}
				}
				else {
					if (changedirection1 == false) {
						m_pEnemySprites[i]->mPosition.x -= 1.2;
						if (m_pEnemySprites[i]->mPosition.x <= 800) {
							changedirection1 = true;
						}
					}
					else if (changedirection1 == true) {
						m_pEnemySprites[i]->mPosition.x += 1.2;
						if (m_pEnemySprites[i]->mPosition.x >= 1400)
							changedirection1 = false;
					}
				}

			}
		}
	}
	
	if (ulDirection & CPlayer::DIR_LEFT)
	{
		m_pSprite->mVelocity.x -= 5.1;
	}
	if (ulDirection & CPlayer::DIR_RIGHT)
	{
		m_pSprite->mVelocity.x += 5.1;
	}

	if (ulDirection & CPlayer::DIR_FORWARD)
	{
		    m_pSprite->mVelocity.y -= 5.1;
	}
	if (ulDirection & CPlayer::DIR_BACKWARD)
		m_pSprite->mVelocity.y += 5.1;

	

}

//void CPlayer::SetNoBullets
void CPlayer::SetDir(int dir)
{
	this->dir = dir;
}

int CPlayer::GetDir()
{
	return this->dir;
}


void CPlayer::CratesPositions()
{
		srand(time(NULL));
	for (int i = 0; i < m_pCrates.size(); i++) {
		m_pCrates[i]->mPosition.y = -rand() % 500 - 1;
		m_pCrates[i]->mPosition.x = rand() % 1450 + 1;
	}

}

void CPlayer::LivesSpawn()
{
	srand(time(NULL));
	for (int i = 0; i < m_pLives.size(); i++) {
		m_pLives[i]->mPosition.x= rand() % 1400 + 50;
		m_pLives[i]->mPosition.y = -rand() % 500 - 1;
	}

}
void CPlayer::SpawnEnemies()
{

	    //if (m_pCrates.size() == 0) {
		    m_bEnemies = true;
			srand(time(NULL));
			for (int i = 0; i < m_pEnemySprites.size(); i++) {
				m_pEnemySprites[i]->mPosition.y = -100;
				m_pEnemySprites[i]->mPosition.x = 300 + i * 900;//rand()%1450+1;
		}

	//}
}


void CPlayer::HeartPositions() {

	m_pHeart1->mPosition = Vec2(1450, 100);
	m_pHeart2->mPosition = Vec2(1450, 175);
	m_pHeart3->mPosition = Vec2(1450, 250);
}


int CPlayer::GetWidth()
{
	return m_pSprite->width();
}
int CPlayer::GetHeight()
{
	return m_pSprite->height();
}



Vec2& CPlayer::Position()
{
	return m_pSprite->mPosition;
}

Vec2& CPlayer::Velocity()
{
	return m_pSprite->mVelocity;
}

void CPlayer::Explode()
{
	m_pExplosionSprite->mPosition = m_pSprite->mPosition;
	m_pExplosionSprite->SetFrame(0);
	PlaySound("data/explosion.wav", NULL, SND_FILENAME | SND_ASYNC);
	m_bExplosion = true;
	lives--;
	SetEmptyHeart();
		
}

bool CPlayer::AdvanceExplosion()
{
	if(m_bExplosion)
	{
		m_pExplosionSprite->SetFrame(m_iExplosionFrame++);
		if(m_iExplosionFrame==m_pExplosionSprite->GetFrameCount())
		{
			m_bExplosion = false;
			m_iExplosionFrame = 0;
			m_pSprite->mVelocity = Vec2(0,0);
			m_eSpeedState = SPEED_STOP;
			return false;
		}
	}

	return true;
}

void CPlayer::SetEmptyHeart()
{
	if (lives == 2)
		m_pHeart1->SetImage("data/heartempty.bmp", RGB(0xff, 0x00, 0xff));
	else if (lives == 1)
		m_pHeart2->SetImage("data/heartempty.bmp", RGB(0xff, 0x00, 0xff));
	else if (lives == 0){
		m_pHeart3->SetImage("data/heartempty.bmp", RGB(0xff, 0x00, 0xff));
	}
	else if(lives==-1){

		MessageBox(0, "Game is Over.", "GameOver", MB_OK);
		PostQuitMessage(0);
	}


}

void CPlayer::bulletcrateCollision()
{
	for (int j = 0; j < m_pBullets.size(); j++) {
		for (int i = 0; i < m_pCrates.size(); i++)
			if (TwoObjectsCollision(m_pBullets[j], m_pCrates[i]) == true)
			{
				m_pCrates.erase(m_pCrates.begin() + i);
				score += 10;
			}
	}

}

void CPlayer::BulletEnemySpaceshipCollision()
{
	for (int j = 0; j < m_pBullets.size(); j++) {
		for (int i = 0; i < m_pEnemySprites.size(); i++)
			if (TwoObjectsCollision(m_pBullets[j], m_pEnemySprites[i]) == true)
			{
				m_pEnemySprites.erase(m_pEnemySprites.begin() + i);
				m_pEnemyBullets.erase(m_pEnemyBullets.begin() + i);
				score += 30;
			}
	}
}

int CPlayer::GetScore() {
	return score;
}


void CPlayer::LivesCollision()
{
	for (int i = 0; i < m_pLives.size(); i++) {
		if (TwoObjectsCollision(m_pSprite, m_pLives[i]) == true) {
			m_pLives.erase(m_pLives.begin() + i);
			if (lives == 2) {
				m_pHeart1->SetImage("data/heart.bmp", RGB(0xff, 0x00, 0xff));
				lives++;
			}
			if (lives == 1) {
				m_pHeart2->SetImage("data/heart.bmp", RGB(0xff, 0x00, 0xff));
				lives++;
			}
			if (lives == 0) {
				m_pHeart3->SetImage("data/heart.bmp", RGB(0xff, 0x00, 0xff));
				lives++;
			}

		}
	}
}

bool CPlayer::EnemyBulletCollision()
{
	for (int i = 0; i < m_pEnemyBullets.size(); i++) {
		if (TwoObjectsCollision(m_pSprite, m_pEnemyBullets[i]) == true) {
			m_pEnemyBullets[i]->mPosition.x = m_pEnemySprites[i]->mPosition.x;
			m_pEnemyBullets[i]->mPosition.y = m_pEnemySprites[i]->mPosition.y + m_pEnemySprites[i]->height() / 2;
			return true;
		}
	}
	return false;
}

bool CPlayer::EnemySpaceshipCollision()
{
	for (int i = 0; i < m_pEnemySprites.size(); i++)
		if (TwoObjectsCollision(m_pSprite, m_pEnemySprites[i]) == true) {
			m_pEnemySprites.erase(m_pEnemySprites.begin() + i);
			m_pEnemyBullets.erase(m_pEnemyBullets.begin() + i);
			return true;
		}
	return false;
}

bool CPlayer::crateCollision()
{
	for (int i = 0; i < m_pCrates.size(); i++)
		if (TwoObjectsCollision(m_pSprite,m_pCrates[i])==true){
			m_pCrates.erase(m_pCrates.begin() + i);
			return true;
		}


	return false;


}
bool CPlayer::TwoObjectsCollision(Sprite* m_pSprite1, Sprite* m_pSprite2)
{
	if ((m_pSprite1->mPosition.x + m_pSprite1->width() / 2 >= m_pSprite2->mPosition.x - m_pSprite2->width() / 2 &&
		m_pSprite1->mPosition.y + m_pSprite1->height() / 2 >= m_pSprite2->mPosition.y - m_pSprite2->height() / 2 &&
		m_pSprite1->mPosition.x - m_pSprite1->width() / 2 <= m_pSprite2->mPosition.x + m_pSprite2->width() / 2) &&
		m_pSprite1->mPosition.y - m_pSprite1->height() / 2 <= m_pSprite2->mPosition.y + m_pSprite2->height() / 2)
		return true;
	else
		return false;

	


}






void CPlayer::RotatePlaneRight()
{
	
	m_pSprite->SetImage("data/spaceshipr.bmp", RGB(0xff, 0x00, 0xff));
	dir = 1;
}
void CPlayer::RotatePlaneDown()
{
	
	m_pSprite->SetImage("data/spaceshipd.bmp", RGB(0xff, 0x00, 0xff));
	dir = 2;
}
void CPlayer::RotatePlaneLeft()
{
	m_pSprite->SetImage("data/spaceshipl.bmp", RGB(0xff, 0x00, 0xff));
	
	dir = 3;
	
}
void CPlayer::RotatePlaneUp()
{
	m_pSprite->SetImage("data/spaceship.bmp", RGB(0xff, 0x00, 0xff));
	
	dir = 0;
}