//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//	   such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _CPLAYER_H_
#define _CPLAYER_H_

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "Sprite.h"
#include <string>
#include<list>
#include<vector>
#include<iostream>
#include <time.h> 
#include <ctime>
//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CPlayer (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class CPlayer
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------
	enum DIRECTION
	{
		DIR_FORWARD = 1,
		DIR_BACKWARD = 2,
		DIR_LEFT = 4,
		DIR_RIGHT = 8,
	};

	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
			 CPlayer(const BackBuffer *pBackBuffer);
	virtual ~CPlayer();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void					Update( float dt );
	void					Draw();
	void					Move(ULONG ulDirection);
	void                    SetDir(int dir);
	int                     GetDir();
	int                     GetWidth();
	int                     GetHeight();
	void                    RotatePlaneRight();
	void                    RotatePlaneDown();
	void                    RotatePlaneLeft();
	void                    RotatePlaneUp();
	Vec2&					Position();
	Vec2&					Velocity();
	void                    Shoot();
	void                    LivesSpawn();
	void                    EnemiesShootings();
	void                    CratesPositions();
	void                    HeartPositions();
	void					Explode();
	bool					AdvanceExplosion();
	void                    SetEmptyHeart();
	void                    bulletcrateCollision();
	int                     GetCrateVSize();
	void                    BulletEnemySpaceshipCollision();
	int                     GetScore();
	bool                    crateCollision();
	void                    LivesCollision();
	bool                    EnemyBulletCollision();
	bool                    EnemySpaceshipCollision();
	bool                    TwoObjectsCollision(Sprite* m_pSprite1,Sprite* m_pSprite2);
	void                    SpawnEnemies();
private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	std::vector<Sprite*>      m_pBullets;
	std::vector<Sprite*>    m_pCrates;
	std::vector<Sprite*>    m_pEnemySprites;
	std::vector<Sprite*>    m_pEnemyBullets;
	std::vector<Sprite*>    m_pLives;
	Sprite*					m_pSprite;
    Sprite*                 m_pBullet;                  
	ESpeedStates			m_eSpeedState;
	float					m_fTimer;
	bool                    m_bBullet;
	bool                    m_bEnemies;
	bool                    m_bEnemyBullets;
	bool changedirection1 = false;
	bool changedirection2 = false;
	bool					m_bExplosion;
	AnimatedSprite*			m_pExplosionSprite;
	int						m_iExplosionFrame;
	int                     dir = 0;
	int                     lives = 3;
	int                     score = 0;
	int                     bullets = 3;
	Sprite*                 m_pHeart1;
	Sprite*                 m_pHeart2;
	Sprite*                 m_pHeart3;
	
};

#endif // _CPLAYER_H_