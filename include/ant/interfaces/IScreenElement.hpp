#ifndef ISCREENELEMENT_HPP_
	#define ISCREENELEMENT_HPP_

#include <ant/core_types.hpp>

namespace ant
{

	class IScreenElement
	{
	public:

		virtual ~IScreenElement();

		virtual HRESULT onRestore() = 0;
		
		virtual HRESULT onLostDevice() = 0;

		virtual HRESULT onRender(ant::DeltaTime fTime, ant::DeltaTime fElapsedTime) = 0;

		virtual void onUpdate(ant::DeltaTime dt) = 0;

		//virtual int VGetZOrder() const = 0;
		//virtual void VSetZOrder(int const zOrder) = 0;
		virtual bool isVisible() const = 0;

		virtual void setVisible(bool visible) = 0;

		virtual LRESULT CALLBACK onMsgProc( AppMsg msg )=0;

		//virtual bool const operator <(IScreenElement const &other) { return VGetZOrder() < other.VGetZOrder(); }
	};
}

#endif