#include "Geo4.h"

CLASS_DECLARATION(ExturbatorRequests);

ExturbatorRequests::ExturbatorRequests() : statusResponseEXT()
{
}


ExturbatorRequests::~ExturbatorRequests()
{
}

void ExturbatorRequests::Initialise(EventManager*const event_manager, ResourceManager*const resourceManager)
{
	Hardware* hw = getParent<Hardware>();

	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &ExturbatorRequests::OnWindowEvent);

	statusResponseEXT.transmitPacket = new CommandDataPacket("EXTRBEXT", PacketClassEnumerator::COMMAND);
	statusResponseEXT.transmitPacket->setPayload(CommandOutStructure{ GET_STATUS });
	statusResponseEXT.responsePacket = new StatusDataPacket(PacketClassEnumerator::STATUS);
	if(hw)hw->RegisterRequest(&statusResponseEXT);

	statusResponseEXT.commit();

	/*
	statusResponseWND.transmitPacket = new CommandDataPacket("EXTRBWND", PacketClassEnumerator::COMMAND);
	statusResponseWND.transmitPacket->setPayload(CommandOutStructure{ GET_STATUS });
	statusResponseWND.responsePacket = new StatusDataPacket(PacketClassEnumerator::STATUS);
	if(hw)hw->RegisterRequest(&statusResponseWND);

	statusResponsePUL.transmitPacket = new CommandDataPacket("EXTRBPUL", PacketClassEnumerator::COMMAND);
	statusResponsePUL.transmitPacket->setPayload(CommandOutStructure{ GET_STATUS });
	statusResponsePUL.responsePacket = new StatusDataPacket(PacketClassEnumerator::STATUS);
	if(hw)hw->RegisterRequest(&statusResponsePUL);

	statusResponseWND.commit();
	statusResponsePUL.commit();
	*/
}

void ExturbatorRequests::Deserialize(CFONode* node)
{
	Entity::Deserialize(node);
}

bool ExturbatorRequests::OnWindowEvent(WindowEvent*const event) {

	Hardware* hw = getParent<Hardware>();
	if (!hw)return 1;

	if (statusResponseEXT.isFinished()
		&& statusResponseEXT.isError()
		&& statusResponseEXT.error == BasePacketRequest::RESPONSE_TIMEOUT) {

		hw->DetectPorts();
		if (hw->IsOpen()) {
			statusResponseEXT.commit();
		}
	}
	//Request status packet again
	if (statusResponseEXT.isCompleted()) {
		hw->setPortValid(true);
		statusResponseEXT.commit();
		//processResponseEXT.commit();
	}

	return 1;
}

void ExturbatorRequests::setExtruderState(bool state, float speed) {

}
void ExturbatorRequests::setPullerState(bool state, float speed) {

}
void ExturbatorRequests::setWinderState(bool state, float speed) {

}