#ifndef IGAMEVIEW_HPP_
	#define IGAMEVIEW_HPP_

#include <ant/core_types.hpp>

namespace ant
{

	enum GameViewType
	{
		GameView_Human,
		GameView_Remote,
		GameView_AI,
		GameView_Recorder,
		GameView_Other
	};

	/**
	 * Interfaces for a game view that describes how that game looks for a certain type of participant, from AI to player
	 */
	class IGameView
	{
	public:
		virtual HRESULT onRestore() = 0;

		virtual void onRender() = 0;

		virtual HRESULT onLostDevice() = 0;

		virtual GameViewType getType() = 0;

		virtual GameViewId getId() = 0;

		virtual void onAttach(GameViewId id, ActorId actorid) = 0;

		virtual LRESULT CALLBACK onMsgProc(AppMsg msg) = 0;

		virtual void onUpdate(ant::DeltaTime dt) = 0;

		virtual ~IGameView(){}
	};
}

#endif