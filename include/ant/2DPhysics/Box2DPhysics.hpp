#ifndef BOX2DPHYSICS_HPP_
	#define BOX2DPHYSICS_HPP_

#include <ant/core_types.hpp>
#include <ant/2DPhysics/IGamePhysics2D.hpp>
#include <ant/interfaces/IGameLogic.hpp>
#include <SFML/Graphics.hpp>
#include <box2d/Box2D.h>
#include <box2d/common/b2Math.h>
#include <set>

#define DEFAULT_GRAVITY 9.82
#define DEFAULT_VELOCITY_ITERATIONS 6 
#define DEFAULT_POSITIONS_ITERATIONS 4

namespace ant
{
	/**
	 * Useful struct for defining material data
	 */
	struct MaterialData
	{
	public:
		MaterialData(ant::Real resitution, ant::Real friction)
		{
			m_restitution = resitution;
			m_friction    = friction;
		}

		MaterialData(const MaterialData& other)
		{
			m_restitution = other.m_restitution;
			m_friction    = other.m_friction;
		}

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	public:
		ant::Real m_restitution;
		ant::Real m_friction;
	};

	/**
	 * Utility functions to convert vectors between SFML and box2d
	 */
	ANT_INLINE sf::Vector2f convertBox2DVec2fTosfVector2f(const b2Vec2& vec)
	{
		return sf::Vector2f(vec.x,vec.y);
	}

	ANT_INLINE b2Vec2 convertsfVector2fToBox2DVec2f(const sf::Vector2f& vec)
	{
		return b2Vec2(vec.x,vec.y);
	}

	/**
	 * Implementation of the 2D physics interface using the box2D engine
	 */
	class Box2DPhysics : public IGamePhysics2D
	{
		// Typedef stuff
		typedef std::map<ActorId, b2Body*> ActorIdToRigidBody;
		typedef std::map<b2Body const *, ActorId> RigidBodyToActorId;
		typedef std::map<std::string, float> DensityTable;
		typedef std::map<std::string, MaterialData> MaterialTable;
		typedef std::pair<b2Body const*, b2Body const*> CollisionPair;
		typedef std::set<CollisionPair> CollisionPairSet;

	public:
		Box2DPhysics();
		virtual ~Box2DPhysics();

		virtual bool initialize() ANT_OVERRIDE;
		virtual void syncVisibleScene() ANT_OVERRIDE;
		virtual void onUpdate(ant::DeltaTime dt) ANT_OVERRIDE;

		// Create physics object
		virtual	void addSphere(ant::Real radius, ActorWeakPtr actor,const std::string& density, const std::string& material, const RigidBodyOptions& options) ANT_OVERRIDE;
		virtual	void addBox(const sf::Vector2f& dimensions, ActorWeakPtr actor, const std::string& density, const std::string& material, const RigidBodyOptions& options) ANT_OVERRIDE;
		virtual void removeActor(const ActorId& id) ANT_OVERRIDE;

		// Physics modifier functions
		virtual void applyForce(const sf::Vector2f& force, const ActorId& id) ANT_OVERRIDE;
		virtual bool kinematicMove(const sf::Vector2f& pos, const ActorId& id) ANT_OVERRIDE;
		
		// Physics states
		virtual void                setVelocity(const ActorId& id, const sf::Vector2f& vel ) ANT_OVERRIDE;
		virtual const sf::Vector2f& getVelocity(const ActorId& id) ANT_OVERRIDE;
		virtual void                setAngularVelocity(const ActorId& id, ant::Real rotVel) ANT_OVERRIDE;
		virtual const ant::Real&    getAngularVelocity(const ActorId& id) ANT_OVERRIDE;		
		virtual void				translate(const ActorId& id, const sf::Vector2f& dx) ANT_OVERRIDE;
		virtual void				rotate(const ActorId& id, ant::Real dr) ANT_OVERRIDE;
		virtual void				setPosition(const ActorId& id, const sf::Vector2f& pos) ANT_OVERRIDE;
		virtual const sf::Vector2f& getPosition(const ActorId& id) ANT_OVERRIDE;
		virtual void                setRotation(const ActorId& id, ant::Real rot) ANT_OVERRIDE;
		virtual const ant::Real&    getRotation(const ActorId& id) ANT_OVERRIDE;
		virtual void                stopActor( const ActorId& id) ANT_OVERRIDE;

	protected:
		void         addB2Shape(ActorStrongPtr pActor, b2FixtureDef fixtureDef, const std::string& material, const std::string& densityStr, const RigidBodyOptions& options);
		MaterialData lookUpMaterialData(const std::string& mat);
		ant::Real    lookUpDensityData(const std::string& mat); // TODO is this even nessecary?
		void         loadMaterialXML();
		b2Body *     findB2Body(const ActorId& id) const;
		ActorId      findActorId(b2Body const * const body) const;
		void         updateDynamicsInformation();
		void         sendCollisionAddEvent(b2Manifold const *manifold, b2Body const * const b1, b2Body const * const b2);
		void         sendCollisionRemoveEvent(b2Body const * const b1, b2Body const * const b2);
		void         removeCollisionObject(b2Body * body);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:	
		DensityTable       m_densityTable;
		MaterialTable      m_materialTable;
		b2World*           m_PhysicsWorld;
		ActorIdToRigidBody m_actorIDToRigidBody;
		RigidBodyToActorId m_rigidBodyToActorID;
		IGameLogic *       m_gameLogic;
		CollisionPairSet   m_previousTickCollisionPairs;
	};
}

#endif