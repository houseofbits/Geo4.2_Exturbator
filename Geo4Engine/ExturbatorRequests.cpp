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
	}
	/*
	if (processResponseEXT.isCompleted()){
		extruderSpeed = processResponseEXT.receivePacket->speed;
		processResponseEXT.commit();
	}
	if (processResponsePUL.isCompleted()){
		diameter.x = processResponseEXT.receivePacket->diameter1;
		diameter.y = processResponseEXT.receivePacket->diameter2;
		processResponsePUL.commit();
	}

	pullerSpeed = calculatePullerSpeedFromExtruder();
	winderSpeed = calculateWinderSpeedFromExtruder();

	setPullerState(pullerState, pullerSpeed);
	setWinderState(winderState, winderSpeed);

	*/

	return 1;
}

void ExturbatorRequests::setExtruderState(bool state, float speed) {
	//if(stateCommandEXT.isCompleted()){
	//stateCommandEXT.transmitPacket->setPayload(StateOutStructure{state, speed});
	//stateCommandEXT.commit();
	//}
}
void ExturbatorRequests::setPullerState(bool state, float speed) {
	//if(stateCommandPUL.isCompleted()){
	//stateCommandPUL.transmitPacket->setPayload(StateOutStructure{state, speed});
	//stateCommandPUL.commit();
	//}
}
void ExturbatorRequests::setWinderState(bool state, float speed) {
	//if(stateCommandWND.isCompleted()){
	//stateCommandWND.transmitPacket->setPayload(StateOutStructure{state, speed});
	//stateCommandWND.commit();
	//}
}
void ExturbatorRequests::setExtruderConfiguration() {
	//if(configCommandEXT.isCompleted()){
	//configCommandEXT.transmitPacket->setPayload(ConfigurationOutStructure{});
	//configCommandEXT.commit();
	//}
}
void ExturbatorRequests::setPullerConfiguration() {

}
void ExturbatorRequests::setWinderConfiguration() {

}
float ExturbatorRequests::calculatePullerSpeedFromExtruder() {

	return 0; 
}
float ExturbatorRequests::calculateWinderSpeedFromExtruder() {

	return 0; 
}