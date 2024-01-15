
void on_topic(
	uxrSession *session,
	uxrObjectId object_id,
	uint16_t request_id,
	uxrStreamId stream_id,
	struct ucdrBuffer *ub,
	uint16_t length,
	void *args)
{
	(void)session;
	(void)object_id;
	(void)request_id;
	(void)stream_id;
	(void)length;

	if (object_id.id == 1)
	{
		AIFaceRecognitionEvent topic;
		AIFaceRecognitionEvent_deserialize_topic(ub, &topic);
		std::cout << "Received : AIFaceRecognitionEvent " << std::endl;

		Json::Value root;
		root["TopicName"] = "AIFaceRecognitionEvent";

		root["AIFaceRecognitionEvent"]["stamp"] = Json::Value(Json::objectValue);
		root["AIFaceRecognitionEvent"]["stamp"]["type"] = "uint32_t";
		root["AIFaceRecognitionEvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["AIFaceRecognitionEvent"]["event"] = Json::Value(Json::objectValue);
		root["AIFaceRecognitionEvent"]["event"]["type"] = "uint32_t";
		root["AIFaceRecognitionEvent"]["event"]["value"] = (uint32_t)topic.event;

		root["AIFaceRecognitionEvent"]["user_id"] = Json::Value(Json::objectValue);
		root["AIFaceRecognitionEvent"]["user_id"]["type"] = "string";
		root["AIFaceRecognitionEvent"]["user_id"]["value"] = std::string(topic.user_id);

		root["AIFaceRecognitionEvent"]["confidence"] = Json::Value(Json::objectValue);
		root["AIFaceRecognitionEvent"]["confidence"]["type"] = "float";
		root["AIFaceRecognitionEvent"]["confidence"]["value"] = topic.confidence;

		root["AIFaceRecognitionEvent"]["spoofing_rate"] = Json::Value(Json::objectValue);
		root["AIFaceRecognitionEvent"]["spoofing_rate"]["type"] = "float";
		root["AIFaceRecognitionEvent"]["spoofing_rate"]["value"] = topic.spoofing_rate;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 2)
	{
		AIFaceRecognitionRequest topic;
		AIFaceRecognitionRequest_deserialize_topic(ub, &topic);
		std::cout << "Received : AIFaceRecognitionRequest " << std::endl;

		Json::Value root;
		root["TopicName"] = "AIFaceRecognitionRequest";

		root["AIFaceRecognitionRequest"]["command"] = Json::Value(Json::objectValue);
		root["AIFaceRecognitionRequest"]["command"]["type"] = "uint32_t";
		root["AIFaceRecognitionRequest"]["command"]["value"] = (uint32_t)topic.command;

		root["AIFaceRecognitionRequest"]["user_id"] = Json::Value(Json::objectValue);
		root["AIFaceRecognitionRequest"]["user_id"]["type"] = "string";
		root["AIFaceRecognitionRequest"]["user_id"]["value"] = std::string(topic.user_id);

		root["AIFaceRecognitionRequest"]["timeout_sec"] = Json::Value(Json::objectValue);
		root["AIFaceRecognitionRequest"]["timeout_sec"]["type"] = "uint16_t";
		root["AIFaceRecognitionRequest"]["timeout_sec"]["value"] = (uint16_t)topic.timeout_sec;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 3)
	{
		AIFaceRecognitionResponse topic;
		AIFaceRecognitionResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : AIFaceRecognitionResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "AIFaceRecognitionResponse";

		root["AIFaceRecognitionResponse"]["command"] = Json::Value(Json::objectValue);
		root["AIFaceRecognitionResponse"]["command"]["type"] = "uint32_t";
		root["AIFaceRecognitionResponse"]["command"]["value"] = (uint32_t)topic.command;

		root["AIFaceRecognitionResponse"]["user_count"] = Json::Value(Json::objectValue);
		root["AIFaceRecognitionResponse"]["user_count"]["type"] = "uint16_t";
		root["AIFaceRecognitionResponse"]["user_count"]["value"] = (uint16_t)topic.user_count;

		root["AIFaceRecognitionResponse"]["code"] = Json::Value(Json::objectValue);
		root["AIFaceRecognitionResponse"]["code"]["type"] = "uint32_t";
		root["AIFaceRecognitionResponse"]["code"]["value"] = (uint32_t)topic.code;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 4)
	{
		AIQREvent topic;
		AIQREvent_deserialize_topic(ub, &topic);
		std::cout << "Received : AIQREvent " << std::endl;

		Json::Value root;
		root["TopicName"] = "AIQREvent";

		root["AIQREvent"]["stamp"] = Json::Value(Json::objectValue);
		root["AIQREvent"]["stamp"]["type"] = "uint32_t";
		root["AIQREvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["AIQREvent"]["event"] = Json::Value(Json::objectValue);
		root["AIQREvent"]["event"]["type"] = "uint32_t";
		root["AIQREvent"]["event"]["value"] = (uint32_t)topic.event;

		root["AIQREvent"]["read_string"] = Json::Value(Json::objectValue);
		root["AIQREvent"]["read_string"]["type"] = "string";
		root["AIQREvent"]["read_string"]["value"] = std::string(topic.read_string);

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 5)
	{
		AIQRRequest topic;
		AIQRRequest_deserialize_topic(ub, &topic);
		std::cout << "Received : AIQRRequest " << std::endl;

		Json::Value root;
		root["TopicName"] = "AIQRRequest";

		root["AIQRRequest"]["command"] = Json::Value(Json::objectValue);
		root["AIQRRequest"]["command"]["type"] = "uint32_t";
		root["AIQRRequest"]["command"]["value"] = (uint32_t)topic.command;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 6)
	{
		AIQRResponse topic;
		AIQRResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : AIQRResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "AIQRResponse";

		root["AIQRResponse"]["command"] = Json::Value(Json::objectValue);
		root["AIQRResponse"]["command"]["type"] = "uint32_t";
		root["AIQRResponse"]["command"]["value"] = (uint32_t)topic.command;

		root["AIQRResponse"]["code"] = Json::Value(Json::objectValue);
		root["AIQRResponse"]["code"]["type"] = "uint32_t";
		root["AIQRResponse"]["code"]["value"] = (uint32_t)topic.code;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 7)
	{
		AISpeakerEvent topic;
		AISpeakerEvent_deserialize_topic(ub, &topic);
		std::cout << "Received : AISpeakerEvent " << std::endl;

		Json::Value root;
		root["TopicName"] = "AISpeakerEvent";

		root["AISpeakerEvent"]["stamp"] = Json::Value(Json::objectValue);
		root["AISpeakerEvent"]["stamp"]["type"] = "uint32_t";
		root["AISpeakerEvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["AISpeakerEvent"]["event"] = Json::Value(Json::objectValue);
		root["AISpeakerEvent"]["event"]["type"] = "uint32_t";
		root["AISpeakerEvent"]["event"]["value"] = (uint32_t)topic.event;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 8)
	{
		AISpeakerRequest topic;
		AISpeakerRequest_deserialize_topic(ub, &topic);
		std::cout << "Received : AISpeakerRequest " << std::endl;

		Json::Value root;
		root["TopicName"] = "AISpeakerRequest";

		root["AISpeakerRequest"]["command"] = Json::Value(Json::objectValue);
		root["AISpeakerRequest"]["command"]["type"] = "uint32_t";
		root["AISpeakerRequest"]["command"]["value"] = (uint32_t)topic.command;

		root["AISpeakerRequest"]["voice_type"] = Json::Value(Json::objectValue);
		root["AISpeakerRequest"]["voice_type"]["type"] = "uint16_t";
		root["AISpeakerRequest"]["voice_type"]["value"] = (uint16_t)topic.voice_type;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 9)
	{
		AISpeakerResponse topic;
		AISpeakerResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : AISpeakerResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "AISpeakerResponse";

		root["AISpeakerResponse"]["command"] = Json::Value(Json::objectValue);
		root["AISpeakerResponse"]["command"]["type"] = "uint32_t";
		root["AISpeakerResponse"]["command"]["value"] = (uint32_t)topic.command;

		root["AISpeakerResponse"]["code"] = Json::Value(Json::objectValue);
		root["AISpeakerResponse"]["code"]["type"] = "uint32_t";
		root["AISpeakerResponse"]["code"]["value"] = (uint32_t)topic.code;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 10)
	{
		AISystemEvent topic;
		AISystemEvent_deserialize_topic(ub, &topic);
		std::cout << "Received : AISystemEvent " << std::endl;

		Json::Value root;
		root["TopicName"] = "AISystemEvent";

		root["AISystemEvent"]["stamp"] = Json::Value(Json::objectValue);
		root["AISystemEvent"]["stamp"]["type"] = "uint32_t";
		root["AISystemEvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["AISystemEvent"]["event"] = Json::Value(Json::objectValue);
		root["AISystemEvent"]["event"]["type"] = "uint32_t";
		root["AISystemEvent"]["event"]["value"] = (uint32_t)topic.event;

		root["AISystemEvent"]["wifi_connection_status"] = Json::Value(Json::objectValue);
		root["AISystemEvent"]["wifi_connection_status"]["type"] = "uint8_t";
		root["AISystemEvent"]["wifi_connection_status"]["value"] = (uint8_t)topic.wifi_connection_status;

		root["AISystemEvent"]["ble_connection_status"] = Json::Value(Json::objectValue);
		root["AISystemEvent"]["ble_connection_status"]["type"] = "uint8_t";
		root["AISystemEvent"]["ble_connection_status"]["value"] = (uint8_t)topic.ble_connection_status;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 11)
	{
		AISystemRequest topic;
		AISystemRequest_deserialize_topic(ub, &topic);
		std::cout << "Received : AISystemRequest " << std::endl;

		Json::Value root;
		root["TopicName"] = "AISystemRequest";

		root["AISystemRequest"]["command"] = Json::Value(Json::objectValue);
		root["AISystemRequest"]["command"]["type"] = "uint32_t";
		root["AISystemRequest"]["command"]["value"] = (uint32_t)topic.command;

		root["AISystemRequest"]["wifi_mode"] = Json::Value(Json::objectValue);
		root["AISystemRequest"]["wifi_mode"]["type"] = "uint8_t";
		root["AISystemRequest"]["wifi_mode"]["value"] = (uint8_t)topic.wifi_mode;

		root["AISystemRequest"]["ble_mode"] = Json::Value(Json::objectValue);
		root["AISystemRequest"]["ble_mode"]["type"] = "uint8_t";
		root["AISystemRequest"]["ble_mode"]["value"] = (uint8_t)topic.ble_mode;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 12)
	{
		AISystemResponse topic;
		AISystemResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : AISystemResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "AISystemResponse";

		root["AISystemResponse"]["command"] = Json::Value(Json::objectValue);
		root["AISystemResponse"]["command"]["type"] = "uint32_t";
		root["AISystemResponse"]["command"]["value"] = (uint32_t)topic.command;

		root["AISystemResponse"]["code"] = Json::Value(Json::objectValue);
		root["AISystemResponse"]["code"]["type"] = "uint32_t";
		root["AISystemResponse"]["code"]["value"] = (uint32_t)topic.code;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 13)
	{
		AIWebRTCEvent topic;
		AIWebRTCEvent_deserialize_topic(ub, &topic);
		std::cout << "Received : AIWebRTCEvent " << std::endl;

		Json::Value root;
		root["TopicName"] = "AIWebRTCEvent";

		root["AIWebRTCEvent"]["stamp"] = Json::Value(Json::objectValue);
		root["AIWebRTCEvent"]["stamp"]["type"] = "uint32_t";
		root["AIWebRTCEvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["AIWebRTCEvent"]["event"] = Json::Value(Json::objectValue);
		root["AIWebRTCEvent"]["event"]["type"] = "uint32_t";
		root["AIWebRTCEvent"]["event"]["value"] = (uint32_t)topic.event;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 14)
	{
		AIWebRTCRequest topic;
		AIWebRTCRequest_deserialize_topic(ub, &topic);
		std::cout << "Received : AIWebRTCRequest " << std::endl;

		Json::Value root;
		root["TopicName"] = "AIWebRTCRequest";

		root["AIWebRTCRequest"]["command"] = Json::Value(Json::objectValue);
		root["AIWebRTCRequest"]["command"]["type"] = "uint32_t";
		root["AIWebRTCRequest"]["command"]["value"] = (uint32_t)topic.command;

		root["AIWebRTCRequest"]["destination"] = Json::Value(Json::objectValue);
		root["AIWebRTCRequest"]["destination"]["type"] = "string";
		root["AIWebRTCRequest"]["destination"]["value"] = std::string(topic.destination);

		root["AIWebRTCRequest"]["timeout_sec"] = Json::Value(Json::objectValue);
		root["AIWebRTCRequest"]["timeout_sec"]["type"] = "uint16_t";
		root["AIWebRTCRequest"]["timeout_sec"]["value"] = (uint16_t)topic.timeout_sec;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 15)
	{
		AIWebRTCResponse topic;
		AIWebRTCResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : AIWebRTCResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "AIWebRTCResponse";

		root["AIWebRTCResponse"]["command"] = Json::Value(Json::objectValue);
		root["AIWebRTCResponse"]["command"]["type"] = "uint32_t";
		root["AIWebRTCResponse"]["command"]["value"] = (uint32_t)topic.command;

		root["AIWebRTCResponse"]["code"] = Json::Value(Json::objectValue);
		root["AIWebRTCResponse"]["code"]["type"] = "uint32_t";
		root["AIWebRTCResponse"]["code"]["value"] = (uint32_t)topic.code;

		root["AIWebRTCResponse"]["destination"] = Json::Value(Json::objectValue);
		root["AIWebRTCResponse"]["destination"]["type"] = "string";
		root["AIWebRTCResponse"]["destination"]["value"] = std::string(topic.destination);

		root["AIWebRTCResponse"]["timeout_sec"] = Json::Value(Json::objectValue);
		root["AIWebRTCResponse"]["timeout_sec"]["type"] = "uint16_t";
		root["AIWebRTCResponse"]["timeout_sec"]["value"] = (uint16_t)topic.timeout_sec;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 16)
	{
		BuzzerEvent topic;
		BuzzerEvent_deserialize_topic(ub, &topic);
		std::cout << "Received : BuzzerEvent " << std::endl;

		Json::Value root;
		root["TopicName"] = "BuzzerEvent";

		root["BuzzerEvent"]["stamp"] = Json::Value(Json::objectValue);
		root["BuzzerEvent"]["stamp"]["type"] = "uint32_t";
		root["BuzzerEvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["BuzzerEvent"]["event"] = Json::Value(Json::objectValue);
		root["BuzzerEvent"]["event"]["type"] = "uint32_t";
		root["BuzzerEvent"]["event"]["value"] = (uint32_t)topic.event;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 17)
	{
		BuzzerRequest topic;
		BuzzerRequest_deserialize_topic(ub, &topic);
		std::cout << "Received : BuzzerRequest " << std::endl;

		Json::Value root;
		root["TopicName"] = "BuzzerRequest";

		root["BuzzerRequest"]["command"] = Json::Value(Json::objectValue);
		root["BuzzerRequest"]["command"]["type"] = "uint32_t";
		root["BuzzerRequest"]["command"]["value"] = (uint32_t)topic.command;

		root["BuzzerRequest"]["buzzer_type"] = Json::Value(Json::objectValue);
		root["BuzzerRequest"]["buzzer_type"]["type"] = "uint16_t";
		root["BuzzerRequest"]["buzzer_type"]["value"] = (uint16_t)topic.buzzer_type;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 18)
	{
		BuzzerResponse topic;
		BuzzerResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : BuzzerResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "BuzzerResponse";

		root["BuzzerResponse"]["command"] = Json::Value(Json::objectValue);
		root["BuzzerResponse"]["command"]["type"] = "uint32_t";
		root["BuzzerResponse"]["command"]["value"] = (uint32_t)topic.command;

		root["BuzzerResponse"]["code"] = Json::Value(Json::objectValue);
		root["BuzzerResponse"]["code"]["type"] = "uint32_t";
		root["BuzzerResponse"]["code"]["value"] = (uint32_t)topic.code;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 19)
	{
		CommonResponse topic;
		CommonResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : CommonResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "CommonResponse";

		root["CommonResponse"]["stamp"] = Json::Value(Json::objectValue);
		root["CommonResponse"]["stamp"]["type"] = "uint32_t";
		root["CommonResponse"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["CommonResponse"]["code"] = Json::Value(Json::objectValue);
		root["CommonResponse"]["code"]["type"] = "uint32_t";
		root["CommonResponse"]["code"]["value"] = (uint32_t)topic.code;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 20)
	{
		FingerPrintEvent topic;
		FingerPrintEvent_deserialize_topic(ub, &topic);
		std::cout << "Received : FingerPrintEvent " << std::endl;

		Json::Value root;
		root["TopicName"] = "FingerPrintEvent";

		root["FingerPrintEvent"]["stamp"] = Json::Value(Json::objectValue);
		root["FingerPrintEvent"]["stamp"]["type"] = "uint32_t";
		root["FingerPrintEvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["FingerPrintEvent"]["event"] = Json::Value(Json::objectValue);
		root["FingerPrintEvent"]["event"]["type"] = "uint32_t";
		root["FingerPrintEvent"]["event"]["value"] = (uint32_t)topic.event;

		root["FingerPrintEvent"]["user_id"] = Json::Value(Json::objectValue);
		root["FingerPrintEvent"]["user_id"]["type"] = "string";
		root["FingerPrintEvent"]["user_id"]["value"] = std::string(topic.user_id);

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 21)
	{
		FingerPrintRequest topic;
		FingerPrintRequest_deserialize_topic(ub, &topic);
		std::cout << "Received : FingerPrintRequest " << std::endl;

		Json::Value root;
		root["TopicName"] = "FingerPrintRequest";

		root["FingerPrintRequest"]["stamp"] = Json::Value(Json::objectValue);
		root["FingerPrintRequest"]["stamp"]["type"] = "uint32_t";
		root["FingerPrintRequest"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["FingerPrintRequest"]["command"] = Json::Value(Json::objectValue);
		root["FingerPrintRequest"]["command"]["type"] = "uint32_t";
		root["FingerPrintRequest"]["command"]["value"] = (uint32_t)topic.command;

		root["FingerPrintRequest"]["user_id"] = Json::Value(Json::objectValue);
		root["FingerPrintRequest"]["user_id"]["type"] = "string";
		root["FingerPrintRequest"]["user_id"]["value"] = std::string(topic.user_id);

		root["FingerPrintRequest"]["timeout_sec"] = Json::Value(Json::objectValue);
		root["FingerPrintRequest"]["timeout_sec"]["type"] = "uint16_t";
		root["FingerPrintRequest"]["timeout_sec"]["value"] = (uint16_t)topic.timeout_sec;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 22)
	{
		FingerPrintResponse topic;
		FingerPrintResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : FingerPrintResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "FingerPrintResponse";

		root["FingerPrintResponse"]["command"] = Json::Value(Json::objectValue);
		root["FingerPrintResponse"]["command"]["type"] = "uint32_t";
		root["FingerPrintResponse"]["command"]["value"] = (uint32_t)topic.command;

		root["FingerPrintResponse"]["user_count"] = Json::Value(Json::objectValue);
		root["FingerPrintResponse"]["user_count"]["type"] = "uint16_t";
		root["FingerPrintResponse"]["user_count"]["value"] = (uint16_t)topic.user_count;

		root["FingerPrintResponse"]["code"] = Json::Value(Json::objectValue);
		root["FingerPrintResponse"]["code"]["type"] = "uint32_t";
		root["FingerPrintResponse"]["code"]["value"] = (uint32_t)topic.code;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 23)
	{
		Log topic;
		Log_deserialize_topic(ub, &topic);
		std::cout << "Received : Log " << std::endl;

		Json::Value root;
		root["TopicName"] = "Log";

		root["Log"]["level"] = Json::Value(Json::objectValue);
		root["Log"]["level"]["type"] = "uint32_t";
		root["Log"]["level"]["value"] = (uint32_t)topic.level;

		root["Log"]["stamp"] = Json::Value(Json::objectValue);
		root["Log"]["stamp"]["type"] = "uint32_t";
		root["Log"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["Log"]["topic"] = Json::Value(Json::objectValue);
		root["Log"]["topic"]["type"] = "string";
		root["Log"]["topic"]["value"] = std::string(topic.topic);

		root["Log"]["value"] = Json::Value(Json::objectValue);
		root["Log"]["value"]["type"] = "string";
		root["Log"]["value"]["value"] = std::string(topic.value);

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 24)
	{
		MorticeEvent topic;
		MorticeEvent_deserialize_topic(ub, &topic);
		std::cout << "Received : MorticeEvent " << std::endl;

		Json::Value root;
		root["TopicName"] = "MorticeEvent";

		root["MorticeEvent"]["stamp"] = Json::Value(Json::objectValue);
		root["MorticeEvent"]["stamp"]["type"] = "uint32_t";
		root["MorticeEvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["MorticeEvent"]["event"] = Json::Value(Json::objectValue);
		root["MorticeEvent"]["event"]["type"] = "uint32_t";
		root["MorticeEvent"]["event"]["value"] = (uint32_t)topic.event;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 25)
	{
		MorticeRequest topic;
		MorticeRequest_deserialize_topic(ub, &topic);
		std::cout << "Received : MorticeRequest " << std::endl;

		Json::Value root;
		root["TopicName"] = "MorticeRequest";

		root["MorticeRequest"]["command"] = Json::Value(Json::objectValue);
		root["MorticeRequest"]["command"]["type"] = "uint32_t";
		root["MorticeRequest"]["command"]["value"] = (uint32_t)topic.command;

		root["MorticeRequest"]["timeout_ms"] = Json::Value(Json::objectValue);
		root["MorticeRequest"]["timeout_ms"]["type"] = "uint16_t";
		root["MorticeRequest"]["timeout_ms"]["value"] = (uint16_t)topic.timeout_ms;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 26)
	{
		MorticeResponse topic;
		MorticeResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : MorticeResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "MorticeResponse";

		root["MorticeResponse"]["command"] = Json::Value(Json::objectValue);
		root["MorticeResponse"]["command"]["type"] = "uint32_t";
		root["MorticeResponse"]["command"]["value"] = (uint32_t)topic.command;

		root["MorticeResponse"]["code"] = Json::Value(Json::objectValue);
		root["MorticeResponse"]["code"]["type"] = "uint32_t";
		root["MorticeResponse"]["code"]["value"] = (uint32_t)topic.code;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 27)
	{
		QRReadEvent topic;
		QRReadEvent_deserialize_topic(ub, &topic);
		std::cout << "Received : QRReadEvent " << std::endl;

		Json::Value root;
		root["TopicName"] = "QRReadEvent";

		root["QRReadEvent"]["stamp"] = Json::Value(Json::objectValue);
		root["QRReadEvent"]["stamp"]["type"] = "uint32_t";
		root["QRReadEvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["QRReadEvent"]["event"] = Json::Value(Json::objectValue);
		root["QRReadEvent"]["event"]["type"] = "uint32_t";
		root["QRReadEvent"]["event"]["value"] = (uint32_t)topic.event;

		root["QRReadEvent"]["read_string"] = Json::Value(Json::objectValue);
		root["QRReadEvent"]["read_string"]["type"] = "string";
		root["QRReadEvent"]["read_string"]["value"] = std::string(topic.read_string);

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 28)
	{
		QRReadRequest topic;
		QRReadRequest_deserialize_topic(ub, &topic);
		std::cout << "Received : QRReadRequest " << std::endl;

		Json::Value root;
		root["TopicName"] = "QRReadRequest";

		root["QRReadRequest"]["command"] = Json::Value(Json::objectValue);
		root["QRReadRequest"]["command"]["type"] = "uint32_t";
		root["QRReadRequest"]["command"]["value"] = (uint32_t)topic.command;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 29)
	{
		QRReadResponse topic;
		QRReadResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : QRReadResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "QRReadResponse";

		root["QRReadResponse"]["command"] = Json::Value(Json::objectValue);
		root["QRReadResponse"]["command"]["type"] = "uint32_t";
		root["QRReadResponse"]["command"]["value"] = (uint32_t)topic.command;

		root["QRReadResponse"]["code"] = Json::Value(Json::objectValue);
		root["QRReadResponse"]["code"]["type"] = "uint32_t";
		root["QRReadResponse"]["code"]["value"] = (uint32_t)topic.code;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 30)
	{
		RFIDReadEvent topic;
		RFIDReadEvent_deserialize_topic(ub, &topic);
		std::cout << "Received : RFIDReadEvent " << std::endl;

		Json::Value root;
		root["TopicName"] = "RFIDReadEvent";

		root["RFIDReadEvent"]["stamp"] = Json::Value(Json::objectValue);
		root["RFIDReadEvent"]["stamp"]["type"] = "uint32_t";
		root["RFIDReadEvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["RFIDReadEvent"]["mode"] = Json::Value(Json::objectValue);
		root["RFIDReadEvent"]["mode"]["type"] = "uint32_t";
		root["RFIDReadEvent"]["mode"]["value"] = (uint32_t)topic.mode;

		root["RFIDReadEvent"]["event"] = Json::Value(Json::objectValue);
		root["RFIDReadEvent"]["event"]["type"] = "uint32_t";
		root["RFIDReadEvent"]["event"]["value"] = (uint32_t)topic.event;

		root["RFIDReadEvent"]["user_id"] = Json::Value(Json::objectValue);
		root["RFIDReadEvent"]["user_id"]["type"] = "string";
		root["RFIDReadEvent"]["user_id"]["value"] = std::string(topic.user_id);

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 31)
	{
		RFIDRequest topic;
		RFIDRequest_deserialize_topic(ub, &topic);
		std::cout << "Received : RFIDRequest " << std::endl;

		Json::Value root;
		root["TopicName"] = "RFIDRequest";

		root["RFIDRequest"]["command"] = Json::Value(Json::objectValue);
		root["RFIDRequest"]["command"]["type"] = "uint32_t";
		root["RFIDRequest"]["command"]["value"] = (uint32_t)topic.command;

		root["RFIDRequest"]["security_key"] = Json::Value(Json::objectValue);
		root["RFIDRequest"]["security_key"]["type"] = "string";
		root["RFIDRequest"]["security_key"]["value"] = std::string(topic.security_key);

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 32)
	{
		RFIDResponse topic;
		RFIDResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : RFIDResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "RFIDResponse";

		root["RFIDResponse"]["command"] = Json::Value(Json::objectValue);
		root["RFIDResponse"]["command"]["type"] = "uint32_t";
		root["RFIDResponse"]["command"]["value"] = (uint32_t)topic.command;

		root["RFIDResponse"]["user_count"] = Json::Value(Json::objectValue);
		root["RFIDResponse"]["user_count"]["type"] = "uint16_t";
		root["RFIDResponse"]["user_count"]["value"] = (uint16_t)topic.user_count;

		root["RFIDResponse"]["code"] = Json::Value(Json::objectValue);
		root["RFIDResponse"]["code"]["type"] = "uint32_t";
		root["RFIDResponse"]["code"]["value"] = (uint32_t)topic.code;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 33)
	{
		SensorEvent topic;
		SensorEvent_deserialize_topic(ub, &topic);
		std::cout << "Received : SensorEvent " << std::endl;

		Json::Value root;
		root["TopicName"] = "SensorEvent";

		root["SensorEvent"]["stamp"] = Json::Value(Json::objectValue);
		root["SensorEvent"]["stamp"]["type"] = "uint32_t";
		root["SensorEvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

		root["SensorEvent"]["type"] = Json::Value(Json::objectValue);
		root["SensorEvent"]["type"]["type"] = "uint32_t";
		root["SensorEvent"]["type"]["value"] = (uint32_t)topic.type;

		root["SensorEvent"]["timeout_sec"] = Json::Value(Json::objectValue);
		root["SensorEvent"]["timeout_sec"]["type"] = "uint16_t";
		root["SensorEvent"]["timeout_sec"]["value"] = (uint16_t)topic.timeout_sec;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 34)
	{
		SensorRequest topic;
		SensorRequest_deserialize_topic(ub, &topic);
		std::cout << "Received : SensorRequest " << std::endl;

		Json::Value root;
		root["TopicName"] = "SensorRequest";

		root["SensorRequest"]["command"] = Json::Value(Json::objectValue);
		root["SensorRequest"]["command"]["type"] = "uint32_t";
		root["SensorRequest"]["command"]["value"] = (uint32_t)topic.command;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 35)
	{
		SensorResponse topic;
		SensorResponse_deserialize_topic(ub, &topic);
		std::cout << "Received : SensorResponse " << std::endl;

		Json::Value root;
		root["TopicName"] = "SensorResponse";

		root["SensorResponse"]["command"] = Json::Value(Json::objectValue);
		root["SensorResponse"]["command"]["type"] = "uint32_t";
		root["SensorResponse"]["command"]["value"] = (uint32_t)topic.command;

		root["SensorResponse"]["code"] = Json::Value(Json::objectValue);
		root["SensorResponse"]["code"]["type"] = "uint32_t";
		root["SensorResponse"]["code"]["value"] = (uint32_t)topic.code;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}
	else if (object_id.id == 36)
	{
		TimeStamp topic;
		TimeStamp_deserialize_topic(ub, &topic);
		std::cout << "Received : TimeStamp " << std::endl;

		Json::Value root;
		root["TopicName"] = "TimeStamp";

		root["TimeStamp"]["sec"] = Json::Value(Json::objectValue);
		root["TimeStamp"]["sec"]["type"] = "uint32_t";
		root["TimeStamp"]["sec"]["value"] = (uint32_t)topic.sec;

		root["TimeStamp"]["nanosec"] = Json::Value(Json::objectValue);
		root["TimeStamp"]["nanosec"]["type"] = "uint32_t";
		root["TimeStamp"]["nanosec"]["value"] = (uint32_t)topic.nanosec;

		Json::FastWriter fastWriter;
		std::string ToPush = fastWriter.write(root);
		fromSub.push(ToPush);
	}

	else
	{
		std::string ToPush("Received unknown topic: ");
		ToPush += std::to_string(object_id.id);
		fromSub.push(ToPush);
		std::cout << ToPush << std::endl;
	}
}
