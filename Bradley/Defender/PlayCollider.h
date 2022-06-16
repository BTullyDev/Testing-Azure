#ifndef PLAY_PLAYCOLLIDER_H
#define PLAY_PLAYCOLLIDER_H

class PlayCollider
{
	public:

		enum EColType
		{
			COL_NONE = 0,		// Ignore collisions for this shape
			COL_CIRCLE,			// Circular (distance) collision [fastest/simplest]
			COL_RECT,			// Rotated rectangular collision [default]
			COL_PIXEL,			// Rotated pixel-based collision [slowest/complicated]
		};

		// Collision shape structure for storing collision data
		struct ColShape
		{
			int shapeId = -1; // Fast way of finding the right shape (should match sprite's id)
			std::string name; // Slow way of finding the right shape
			int colType = COL_RECT;
			Point2f origin{ 0.0f, 0.0f }; // origin and centre of rotation
			Point2f offset[4]{}; // offsets from origin to corners of rect (where required)
			float radius = 0.0f; // circle radius (where required)
		};

		struct ColInstance
		{
			int instanceId = -1;
			ColShape* pColShape;
			Point2f pos;
			float rot;
			Point2f r[4]; // rotated corners of rect (where required)
			bool bColliding = false;
		};

		struct ColList
		{
			int instanceId = -1;
			std::vector< int > vCollisions;
		};


		static PlayCollider& Instance();
		static PlayCollider& Instance( const char* path );
		static void Destroy();

		int GetShapeId( const char* name ) const;

		int CreateCollisionInstance( Point2f position, float rotation, int shapeId );
		void UpdateCollisionInstance( int instanceID, Point2f position, float rotation );

		// ProcessCollisions returns a vector of pairs which maps each actor's id to a vector containing the candidate ids that collided with it 
		std::vector < ColList > ProcessCollisions( std::vector<int>& vActors, std::vector<int>& vCandidates );
		
		bool CheckCollision( int instanceId1, int instanceId2 );
		bool CheckCollision( ColInstance & cInstance1, ColInstance & cInstance2 );
		void DrawAllColliderShapes( PlayGraphics* pb);


		
	private:

		PlayCollider( const char* path ); // Basic constructor
		~PlayCollider();
		PlayCollider& operator=( const PlayCollider& ) = delete;   //  prevent assign
		PlayCollider( const PlayCollider& ) = delete; // prevent copy

		bool CheckCircleCircle( ColInstance & cInstance1, ColInstance & cInstance2 );
		bool CheckCircleRect( ColInstance & cCircleInstance, ColInstance & cRectInstance );
		bool CheckRectRect( ColInstance & cInstance1, ColInstance & cInstance2 );
		bool CheckSeparationAxis( ColInstance & cRectInstance1, ColInstance & cRectInstance2, const Vector2f& axis);

		std::string m_colliderPath;

		std::vector< ColShape > vShapeData;
		std::vector< ColInstance > vInstanceData;
		int m_totalShapes{ 0 };
		int m_totalInstances{ 0 };
		int m_totalInstancesEver{ 0 };

		bool m_bInitialised{ 0 };
		static PlayCollider* s_pInstance;

};

#endif

