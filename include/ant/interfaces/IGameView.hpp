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
	 * Interfaces for a game view that describes how tha game looks for a ceartin type of participant, from AI to player
	 */
	class IGameView
	{
		virtual HRESULT onRestore() = 0;

		virtual void onRender() = 0;

		virtual HRESULT onLostDevice() = 0;

		virtual GameViewType getType() = 0;

		virtual GameViewId getId() = 0;

		virtual void onAttach(GameViewId id, ActorId id) = 0;

		virtual LRESULT CALLBACK onMsgProc(AppMsg msg) = 0;

		virtual void onUpdate(ant::DeltaTime dt) = 0;

		virtual ~IGameView(){}
	};
}

#endif