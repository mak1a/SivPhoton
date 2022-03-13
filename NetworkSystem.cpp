# define NOMINMAX
# include <LoadBalancing-cpp/inc/Client.h>
# include "NetworkSystem.hpp"


namespace s3d
{
	namespace detail
	{
		[[nodiscard]]
		static String ToString(const ExitGames::Common::JString& str)
		{
			return Unicode::FromWstring(std::wstring_view{ str.cstr(), str.length() });
		}

		[[nodiscard]]
		static ExitGames::Common::JString ToJString(const StringView s)
		{
			return ExitGames::Common::JString{ Unicode::ToWstring(s).c_str() };
		}
	}

	template <class Type, uint8 customTypeIndex>
	class SivCustomType : public ExitGames::Common::CustomType<SivCustomType<Type, customTypeIndex>, customTypeIndex>
	{
	public:

		SIV3D_NODISCARD_CXX20
		SivCustomType() = default;

		SIV3D_NODISCARD_CXX20
		explicit SivCustomType(const Type& value)
			: ExitGames::Common::CustomType<SivCustomType<Type, customTypeIndex>, customTypeIndex>{}
			, m_value{ value } {}

		SIV3D_NODISCARD_CXX20
		SivCustomType(const SivCustomType& toCopy)
			: ExitGames::Common::CustomType<SivCustomType<Type, customTypeIndex>, customTypeIndex>{}
			, m_value{ toCopy.m_value } {}

		virtual ~SivCustomType() {}

		SivCustomType& operator=(const SivCustomType& toCopy)
		{
			m_value = toCopy.m_value;
			return *this;
		}

		void cleanup() {}

		bool compare(const ExitGames::Common::CustomTypeBase& other) const override
		{
			return m_value == ((SivCustomType&)other).m_value;
		}

		void duplicate(ExitGames::Common::CustomTypeBase* pRetVal) const override
		{
			*reinterpret_cast<SivCustomType*>(pRetVal) = *this;
		}

		void deserialize(const nByte* pData, [[maybe_unused]] const short length) override
		{
			std::memcpy(&m_value, pData, sizeof(Type));
		}

		short serialize(nByte* pRetVal) const override
		{
			if (pRetVal)
			{
				Type* data = reinterpret_cast<Type*>(pRetVal);
				std::memcpy(data, &m_value, sizeof(Type));
				pRetVal = reinterpret_cast<nByte*>(data);
			}

			return sizeof(Type);
		}

		ExitGames::Common::JString& toString(ExitGames::Common::JString& retStr, [[maybe_unused]] const bool withTypes = false) const override
		{
			return retStr = detail::ToJString(Format(m_value));
		}

		Type& getValue()
		{
			return m_value;
		}

	private:

		Type m_value{};
	};


	// Color関連
	using PhotonColorF = SivCustomType<ColorF, 0>;
	using PhotonColor = SivCustomType<Color, 1>;
	using PhotonHSV = SivCustomType<HSV, 2>;

	// 座標関連
	using PhotonPoint = SivCustomType<Point, 3>;
	using PhotonVec2 = SivCustomType<Vec2, 4>;
	using PhotonVec3 = SivCustomType<Vec3, 5>;
	using PhotonVec4 = SivCustomType<Vec4, 6>;
	using PhotonFloat2 = SivCustomType<Float2, 7>;
	using PhotonFloat3 = SivCustomType<Float3, 8>;
	using PhotonFloat4 = SivCustomType<Float4, 9>;
	using PhotonMat3x2 = SivCustomType<Mat3x2, 10>;


	// 図形関連
	using PhotonRect = SivCustomType<Rect, 11>;
	using PhotonCircle = SivCustomType<Circle, 12>;
	using PhotonLine = SivCustomType<Line, 13>;
	using PhotonTriangle = SivCustomType<Triangle, 14>;
	using PhotonRectF = SivCustomType<RectF, 15>;
	using PhotonQuad = SivCustomType<Quad, 16>;
	using PhotonEllipse = SivCustomType<Ellipse, 17>;
	using PhotonRoundRect = SivCustomType<RoundRect, 18>;

	// Byte型
	using PhotonByte = SivCustomType<Byte, 19>;
}

namespace s3d
{
	class SivPhoton::SivPhotonDetail : public ExitGames::LoadBalancing::Listener
	{
	public:

		explicit SivPhotonDetail(SivPhoton& context)
			: m_context{ context }
		{
			m_receiveEventFunctions.emplace(uint8{ 0 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<ColorF, 0>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 1 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Color, 1>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 2 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<HSV, 2>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 3 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Point, 3>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 4 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Vec2, 4>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 5 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Vec3, 5>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 6 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Vec4, 6>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 7 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Float2, 7>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 8 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Float3, 8>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 9 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Float4, 9>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 10 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Mat3x2, 10>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 11 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Rect, 11>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 12 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Circle, 12>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 13 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Line, 13>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 14 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Triangle, 14>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 15 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<RectF, 15>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 16 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Quad, 16>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 17 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Ellipse, 17>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 18 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<RoundRect, 18>(playerID, eventCode, data); });
			
			m_receiveArrayEventFunctions.emplace(uint8{ 0 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<ColorF, 0>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 1 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Color, 1>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 2 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<HSV, 2>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 3 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Point, 3>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 4 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Vec2, 4>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 5 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Vec3, 5>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 6 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Vec4, 6>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 7 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Float2, 7>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 8 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Float3, 8>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 9 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Float4, 9>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 19 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayByte<Byte, 19>(playerID, eventCode, data); });
		}

		void debugReturn([[maybe_unused]] const int debugLevel, [[maybe_unused]] const ExitGames::Common::JString& string) override
		{

		}

		void connectionErrorReturn(const int errorCode) override
		{
			m_context.connectionErrorReturn(errorCode);
			m_context.m_isUsePhoton = false;
		}

		void clientErrorReturn([[maybe_unused]] const int errorCode) override
		{

		}

		void warningReturn([[maybe_unused]] const int warningCode) override
		{

		}

		void serverErrorReturn([[maybe_unused]] const int errorCode) override
		{

		}

		// 自分でも他人でも、誰かが参加したら呼ばれるコールバック
		void joinRoomEventAction(const int playerID, const ExitGames::Common::JVector<int>& playerIDs, const ExitGames::LoadBalancing::Player& player) override
		{
			Array<int32> ids(playerIDs.getSize());
			for (unsigned i = 0; i < playerIDs.getSize(); ++i)
			{
				ids[i] = playerIDs[i];
			}

			const auto myID = m_context.getClient().getLocalPlayer().getNumber();
			const auto newID = player.getNumber();
			const bool isSelf = (myID == newID);
			m_context.joinRoomEventAction(playerID, ids, isSelf);
		}

		// 他人でも、誰かが退室したら呼ばれるコールバック
		void leaveRoomEventAction(const int playerID, const bool isInactive) override
		{
			m_context.leaveRoomEventAction(playerID, isInactive);
		}

		// ルームで他人が RaiseEvent したら呼ばれるコールバック
		void customEventAction(const int playerID, const nByte eventCode, const ExitGames::Common::Object& _data) override
		{
			uint8 type = _data.getType();

			if (type == ExitGames::Common::TypeCode::CUSTOM)
			{
				const uint8 customType = _data.getCustomType();
				m_receiveEventFunctions[customType](playerID, eventCode, _data);
				return;
			}

			if (type == ExitGames::Common::TypeCode::HASHTABLE)
			{
				ExitGames::Common::Hashtable eventDataContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(_data).getDataCopy();
				ExitGames::Common::JString arrayType = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventDataContent.getValue(L"ArrayType")).getDataCopy();
				if (arrayType == L"Array")
				{
					type = eventDataContent.getValue(L"values")->getType();
					if (type == ExitGames::Common::TypeCode::CUSTOM)
					{
						const uint8 customType = eventDataContent.getValue(L"values")->getCustomType();
						m_receiveArrayEventFunctions[customType](playerID, eventCode, eventDataContent.getValue(L"values"));
						return;
					}

					switch (type)
					{
					case ExitGames::Common::TypeCode::INTEGER:
					{
						auto values = ExitGames::Common::ValueObject<int*>(eventDataContent.getValue(L"values")).getDataCopy();
						auto length = *(ExitGames::Common::ValueObject<int*>(eventDataContent.getValue(L"values"))).getSizes();

						Array<int32> data(length);
						for (size_t i = 0; i < length; ++i)
						{
							data[i] = values[i];
						}
						m_context.customEventAction(playerID, eventCode, data);
						return;
					}
					case ExitGames::Common::TypeCode::DOUBLE:
					{
						auto values = ExitGames::Common::ValueObject<double*>(eventDataContent.getValue(L"values")).getDataCopy();
						auto length = *(ExitGames::Common::ValueObject<double*>(eventDataContent.getValue(L"values"))).getSizes();

						Array<double> data(length);
						for (size_t i = 0; i < length; ++i)
						{
							data[i] = values[i];
						}
						m_context.customEventAction(playerID, eventCode, data);
						return;
					}
					case ExitGames::Common::TypeCode::FLOAT:
					{
						auto values = ExitGames::Common::ValueObject<float*>(eventDataContent.getValue(L"values")).getDataCopy();
						auto length = *(ExitGames::Common::ValueObject<float*>(eventDataContent.getValue(L"values"))).getSizes();

						Array<float> data(length);
						for (size_t i = 0; i < length; ++i)
						{
							data[i] = values[i];
						}
						m_context.customEventAction(playerID, eventCode, data);
						return;
					}
					case ExitGames::Common::TypeCode::BOOLEAN:
					{
						auto values = ExitGames::Common::ValueObject<bool*>(eventDataContent.getValue(L"values")).getDataCopy();
						auto length = *(ExitGames::Common::ValueObject<bool*>(eventDataContent.getValue(L"values"))).getSizes();

						Array<bool> data(length);
						for (size_t i = 0; i < length; ++i)
						{
							data[i] = values[i];
						}
						m_context.customEventAction(playerID, eventCode, data);
						return;
					}
					case ExitGames::Common::TypeCode::STRING:
					{
						auto values = ExitGames::Common::ValueObject<ExitGames::Common::JString*>(eventDataContent.getValue(L"values")).getDataCopy();
						auto length = *(ExitGames::Common::ValueObject<ExitGames::Common::JString*>(eventDataContent.getValue(L"values"))).getSizes();

						Array<String> data(length);
						for (size_t i = 0; i < length; ++i)
						{
							// hatena
							data[i] = detail::ToString(values[i]);
						}
						m_context.customEventAction(playerID, eventCode, data);
						return;
					}
					default:
						break;
					}
				}

				return;
			}

			switch (type)
			{
			case ExitGames::Common::TypeCode::INTEGER:
				m_context.customEventAction(playerID, eventCode, ExitGames::Common::ValueObject<int>(_data).getDataCopy());
				return;
			case ExitGames::Common::TypeCode::DOUBLE:
				m_context.customEventAction(playerID, eventCode, ExitGames::Common::ValueObject<double>(_data).getDataCopy());
				return;
			case ExitGames::Common::TypeCode::FLOAT:
				m_context.customEventAction(playerID, eventCode, ExitGames::Common::ValueObject<float>(_data).getDataCopy());
				return;
			case ExitGames::Common::TypeCode::BOOLEAN:
				m_context.customEventAction(playerID, eventCode, ExitGames::Common::ValueObject<bool>(_data).getDataCopy());
				return;
			case ExitGames::Common::TypeCode::STRING:
				m_context.customEventAction(playerID, eventCode, detail::ToString(ExitGames::Common::ValueObject<ExitGames::Common::JString>(_data).getDataCopy()));
				return;
			default:
				break;
			}
		}

		// connect() の結果を通知するコールバック
		void connectReturn(const int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster) override
		{
			const String errorText = detail::ToString(errorString);
			const String regionText = detail::ToString(region);
			const String clusterText = detail::ToString(cluster);
			m_context.connectReturn(errorCode, errorText, regionText, clusterText);
			if (errorCode)
			{
				m_context.m_isUsePhoton = false;
			}
		}

		// disconnect() の結果を通知するコールバック
		void disconnectReturn() override
		{
			m_context.disconnectReturn();
			m_context.m_isUsePhoton = false;
		}

		// 
		void leaveRoomReturn(const int errorCode, const ExitGames::Common::JString& errorString) override
		{
			const String errorText = detail::ToString(errorString);
			m_context.leaveRoomReturn(errorCode, errorText);
		}

		void joinRandomRoomReturn(const int localPlayerID, [[maybe_unused]] const ExitGames::Common::Hashtable& roomProperties, [[maybe_unused]] const ExitGames::Common::Hashtable& playerProperties, const int errorCode, const ExitGames::Common::JString& errorString) override
		{
			m_context.joinRandomRoomReturn(localPlayerID, errorCode, detail::ToString(errorString));
		}

		void createRoomReturn(const int localPlayerID, [[maybe_unused]] const ExitGames::Common::Hashtable& roomProperties, [[maybe_unused]] const ExitGames::Common::Hashtable& playerProperties, const int errorCode, const ExitGames::Common::JString& errorString) override
		{
			m_context.createRoomReturn(localPlayerID, errorCode, detail::ToString(errorString));
		}

	private:

		SivPhoton& m_context;

		HashTable<uint8, std::function<void(const int, const nByte, const ExitGames::Common::Object&)>> m_receiveEventFunctions;

		HashTable<uint8, std::function<void(const int, const nByte, const ExitGames::Common::Object*)>> m_receiveArrayEventFunctions;

		template <class Type, uint8 N>
		void receivedCustomType(const int playerID, const nByte eventCode, const ExitGames::Common::Object& eventContent)
		{
			auto value = ExitGames::Common::ValueObject<SivCustomType<Type, N>>(eventContent).getDataCopy().getValue();
			m_context.customEventAction(playerID, eventCode, value);
		}

		template <class Type, uint8 N>
		void receivedCustomArrayType(const int playerID, const nByte eventCode, const ExitGames::Common::Object* eventContent)
		{
			SivCustomType<Type, N>* values = ExitGames::Common::ValueObject<SivCustomType<Type, N>*>(eventContent).getDataCopy();
			auto length = *(ExitGames::Common::ValueObject<SivCustomType<Type, N>*>(eventContent)).getSizes();

			Array<Type> data(length);
			for (size_t i = 0; i < length; ++i)
			{
				data[i] = values[i].getValue();
			}
			m_context.customEventAction(playerID, eventCode, data);
		}

		template <class Type, uint8 N>
		void receivedCustomArrayByte(const int playerID, const nByte eventCode, const ExitGames::Common::Object* eventContent)
		{
			SivCustomType<Type, N>* values = ExitGames::Common::ValueObject<SivCustomType<Type, N>*>(eventContent).getDataCopy();
			auto length = *(ExitGames::Common::ValueObject<SivCustomType<Type, N>*>(eventContent)).getSizes();

			Array<Type> data(length);
			for (size_t i = 0; i < length; ++i)
			{
				data[i] =  values[i].getValue();
			}

			Deserializer<MemoryReader> reader{ data.data(), data.size() };
			m_context.customEventAction(playerID, eventCode, reader);
		}
	};
}

namespace s3d
{
	SivPhoton::SivPhoton(const StringView secretPhotonAppID, const StringView photonAppVersion, const bool verbose)
		: m_listener{ std::make_unique<SivPhotonDetail>(*this) }
		, m_client{ std::make_unique<ExitGames::LoadBalancing::Client>(*m_listener, detail::ToJString(secretPhotonAppID), detail::ToJString(photonAppVersion)) }
		, m_verbose{ verbose }
		, m_isUsePhoton{ false }
	{

		PhotonColorF::registerType();
		PhotonColor::registerType();
		PhotonHSV::registerType();
		PhotonPoint::registerType();
		PhotonVec2::registerType();
		PhotonVec3::registerType();
		PhotonVec4::registerType();
		PhotonFloat2::registerType();
		PhotonFloat3::registerType();
		PhotonFloat4::registerType();
		PhotonMat3x2::registerType();
		PhotonRect::registerType();
		PhotonCircle::registerType();
		PhotonLine::registerType();
		PhotonTriangle::registerType();
		PhotonRectF::registerType();
		PhotonQuad::registerType();
		PhotonEllipse::registerType();
		PhotonRoundRect::registerType();
		PhotonByte::registerType();
	}

	SivPhoton::~SivPhoton()
	{
		PhotonColorF::unregisterType();
		PhotonColor::unregisterType();
		PhotonHSV::unregisterType();
		PhotonPoint::unregisterType();
		PhotonVec2::unregisterType();
		PhotonVec3::unregisterType();
		PhotonVec4::unregisterType();
		PhotonFloat2::unregisterType();
		PhotonFloat3::unregisterType();
		PhotonFloat4::unregisterType();
		PhotonMat3x2::unregisterType();
		PhotonRect::unregisterType();
		PhotonCircle::unregisterType();
		PhotonLine::unregisterType();
		PhotonTriangle::unregisterType();
		PhotonRectF::unregisterType();
		PhotonQuad::unregisterType();
		PhotonEllipse::unregisterType();
		PhotonRoundRect::unregisterType();
		PhotonByte::unregisterType();

		disconnect();
	}

	void SivPhoton::init(const StringView secretPhotonAppID, const StringView photonAppVersion, const bool verbose)
	{
		m_listener = std::make_unique<SivPhotonDetail>(*this);
		m_client = std::make_unique<ExitGames::LoadBalancing::Client>(*m_listener, detail::ToJString(secretPhotonAppID), detail::ToJString(photonAppVersion));
		m_verbose = verbose;
		m_isUsePhoton = false;

		// デフォルトコンストラクタを使用していた事を想定
		PhotonColorF::registerType();
		PhotonColor::registerType();
		PhotonHSV::registerType();
		PhotonPoint::registerType();
		PhotonVec2::registerType();
		PhotonVec3::registerType();
		PhotonVec4::registerType();
		PhotonFloat2::registerType();
		PhotonFloat3::registerType();
		PhotonFloat4::registerType();
		PhotonMat3x2::registerType();
		PhotonRect::registerType();
		PhotonCircle::registerType();
		PhotonLine::registerType();
		PhotonTriangle::registerType();
		PhotonRectF::registerType();
		PhotonQuad::registerType();
		PhotonEllipse::registerType();
		PhotonRoundRect::registerType();
		PhotonByte::registerType();
	}

	void SivPhoton::connect(const StringView userName, const Optional<String>& defaultRoomName)
	{
		m_defaultRoomName = defaultRoomName.value_or(String{ userName });

		const auto userNameJ = detail::ToJString(userName);
		const auto userID = ExitGames::LoadBalancing::AuthenticationValues{}
		.setUserID(userNameJ + GETTIMEMS());

		if (not m_client->connect({ userID, userNameJ }))
		{
			if (m_verbose)
			{
				Print << U"[SivPhoton] ExitGmae::LoadBalancing::Client::connect() failed.";
			}
			return;
		}

		m_client->fetchServerTimestamp();
		m_isUsePhoton = true;
	}

	void SivPhoton::disconnect()
	{
		m_client->disconnect();
	}

	void SivPhoton::update()
	{
		m_client->service();
	}

	void SivPhoton::opJoinRandomRoom(const int32 maxPlayers)
	{
		if (not InRange(maxPlayers, 0, 255))
		{
			return;
		}

		m_client->opJoinRandomRoom({}, static_cast<uint8>(Clamp(maxPlayers, 1, 255)));
	}

	void SivPhoton::opJoinRoom(const StringView roomName, const bool rejoin)
	{
		const auto roomNameJ = detail::ToJString(roomName);

		m_client->opJoinRoom(roomNameJ, rejoin);
	}

	void SivPhoton::opCreateRoom(const StringView roomName, const int32 maxPlayers)
	{
		if (not InRange(maxPlayers, 0, 255))
		{
			return;
		}

		const auto roomNameJ = detail::ToJString(roomName);
		const auto roomOption = ExitGames::LoadBalancing::RoomOptions()
			.setMaxPlayers(static_cast<uint8>(Clamp(maxPlayers, 1, 255)));

		m_client->opCreateRoom(roomNameJ, roomOption);
	}

	void SivPhoton::opLeaveRoom()
	{
		constexpr bool willComeBack = false;

		m_client->opLeaveRoom(willComeBack);
	}
}

namespace s3d
{
	void SivPhoton::opRaiseEvent(const uint8 eventCode, const ColorF& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonColorF{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Color& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonColor{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const HSV& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonHSV{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Point& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonPoint{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Vec2& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonVec2{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Vec3& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonVec3{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Vec4& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonVec4{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Float2& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonFloat2{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Float3& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonFloat3{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Float4& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonFloat4{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Mat3x2& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonMat3x2{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Rect& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonRect{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Circle& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonCircle{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Line& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonLine{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Triangle& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonTriangle{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const RectF& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonRectF{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Quad& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonQuad{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Ellipse& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonEllipse{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const RoundRect& value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, PhotonRoundRect{ value }, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<ColorF>& values)
	{
		constexpr bool reliable = true;

		Array<PhotonColorF> data;
		data.reserve(values.size());

		for (const auto& v : values)
		{
			data.emplace_back(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<Color>& values)
	{
		constexpr bool reliable = true;

		Array<PhotonColor> data;
		data.reserve(values.size());

		for (const auto& v : values)
		{
			data.emplace_back(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<HSV>& values)
	{
		constexpr bool reliable = true;

		Array<PhotonHSV> data;
		data.reserve(values.size());

		for (const auto& v : values)
		{
			data.emplace_back(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<Point>& values)
	{
		constexpr bool reliable = true;

		Array<PhotonPoint> data;
		data.reserve(values.size());

		for (const auto& v : values)
		{
			data.emplace_back(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<Vec2>& values)
	{
		constexpr bool reliable = true;

		Array<PhotonVec2> data;
		data.reserve(values.size());

		for (const auto& v : values)
		{
			data.emplace_back(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<Vec3>& values)
	{
		constexpr bool reliable = true;

		Array<PhotonVec3> data;
		data.reserve(values.size());

		for (const auto& v : values)
		{
			data.emplace_back(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<Vec4>& values)
	{
		constexpr bool reliable = true;

		Array<PhotonVec4> data;
		data.reserve(values.size());

		for (const auto& v : values)
		{
			data.emplace_back(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<Float2>& values)
	{
		constexpr bool reliable = true;

		Array<PhotonFloat2> data;
		data.reserve(values.size());

		for (const auto& v : values)
		{
			data.emplace_back(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<Float3>& values)
	{
		constexpr bool reliable = true;

		Array<PhotonFloat3> data;
		data.reserve(values.size());

		for (const auto& v : values)
		{
			data.emplace_back(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<Float4>& values)
	{
		constexpr bool reliable = true;

		Array<PhotonFloat4> data;
		data.reserve(values.size());

		for (const auto& v : values)
		{
			data.emplace_back(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, Serializer<MemoryWriter>& writer)
	{
		constexpr bool reliable = true;

		Array<PhotonByte> data;
		for (const auto& arrayByte = writer->getBlob().asArray(); const auto& v : arrayByte)
		{
			data << PhotonByte{ v };
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const int32 value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, value, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const double value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, value, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const float value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, value, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const bool value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, value, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const StringView value)
	{
		constexpr bool reliable = true;
		m_client->opRaiseEvent(reliable, detail::ToJString(value), eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<int32>& values)
	{
		constexpr bool reliable = true;

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", values.data(), static_cast<int16>(values.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<double>& values)
	{
		constexpr bool reliable = true;

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", values.data(), static_cast<int16>(values.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<float>& values)
	{
		constexpr bool reliable = true;

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", values.data(), static_cast<int16>(values.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<bool>& values)
	{
		constexpr bool reliable = true;

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", values.data(), static_cast<int16>(values.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	void SivPhoton::opRaiseEvent(const uint8 eventCode, const Array<String>& values)
	{
		constexpr bool reliable = true;

		Array<ExitGames::Common::JString> data;
		for (const auto& v : values)
		{
			data << detail::ToJString(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(reliable, ev, eventCode);
	}

	String SivPhoton::getUserName() const
	{
		return detail::ToString(m_client->getLocalPlayer().getName());
	}

	String SivPhoton::getUserID() const
	{
		return detail::ToString(m_client->getLocalPlayer().getUserID());
	}

	Array<String> SivPhoton::getRoomNameList() const
	{
		const auto roomNameList = m_client->getRoomNameList();
		Array<String> result;

		for (uint32 i = 0; i < roomNameList.getSize(); ++i)
		{
			result << detail::ToString(roomNameList[i]);
		}

		return result;
	}

	bool SivPhoton::isInRoom() const
	{
		return m_client->getIsInGameRoom();
	}

	String SivPhoton::getCurrentRoomName() const
	{
		if (not m_client->getIsInGameRoom())
		{
			return{};
		}

		return detail::ToString(m_client->getCurrentlyJoinedRoom().getName());
	}

	int32 SivPhoton::getPlayerCountInCurrentRoom() const
	{
		if (not m_client->getIsInGameRoom())
		{
			return 0;
		}

		return m_client->getCurrentlyJoinedRoom().getPlayerCount();
	}

	int32 SivPhoton::getMaxPlayersInCurrentRoom() const
	{
		if (not m_client->getIsInGameRoom())
		{
			return 0;
		}

		return m_client->getCurrentlyJoinedRoom().getMaxPlayers();
	}

	bool SivPhoton::getIsOpenInCurrentRoom() const
	{
		return m_client->getCurrentlyJoinedRoom().getIsOpen();
	}

	bool SivPhoton::getIsVisibleInCurrentRoom() const
	{
		return m_client->getCurrentlyJoinedRoom().getIsVisible();
	}

	void SivPhoton::setIsOpenInCurrentRoom(const bool isOpen)
	{
		m_client->getCurrentlyJoinedRoom().setIsOpen(isOpen);
	}

	void SivPhoton::setIsVisibleInCurrentRoom(const bool isVisible)
	{
		m_client->getCurrentlyJoinedRoom().setIsVisible(isVisible);
	}

	int32 SivPhoton::getCountGamesRunning() const
	{
		return m_client->getCountGamesRunning();
	}

	int32 SivPhoton::getCountPlayersIngame() const
	{
		return m_client->getCountPlayersIngame();
	}

	int32 SivPhoton::getCountPlayersOnline() const
	{
		return m_client->getCountPlayersOnline();
	}

	Optional<int32> SivPhoton::localPlayerID() const
	{
		const int32 localPlayerID = m_client->getLocalPlayer().getNumber();

		if (localPlayerID < 0)
		{
			return none;
		}

		return localPlayerID;
	}

	bool SivPhoton::isMasterClient() const
	{
		return m_client->getLocalPlayer().getIsMasterClient();
	}

	bool SivPhoton::isUsePhoton() const noexcept
	{
		return m_isUsePhoton;
	}

	void SivPhoton::connectionErrorReturn(const int32 errorCode)
	{
		if (m_verbose)
		{
			Print << U"[SivPhoton] SivPhoton::connectionErrorReturn() [サーバへの接続が失敗したときに呼ばれる]";
			Print << U"- [SivPhoton] errorCode: " << errorCode;
		}
	}

	void SivPhoton::connectReturn([[maybe_unused]] const int32 errorCode, const String& errorString, const String& region, const String& cluster)
	{
		if (m_verbose)
		{
			Print << U"[SivPhoton] SivPhoton::connectReturn()";
			Print << U"- [SivPhoton] error: " << errorString;
			Print << U"- [SivPhoton] region: " << region;
			Print << U"- [SivPhoton] cluster: " << cluster;
		}
	}

	void SivPhoton::disconnectReturn()
	{
		if (m_verbose)
		{
			Print << U"[SivPhoton] SivPhoton::disconnectReturn() [サーバから切断されたときに呼ばれる]";
		}
	}

	void SivPhoton::leaveRoomReturn(const int32 errorCode, const String& errorString)
	{
		if (m_verbose)
		{
			Print << U"[SivPhoton] SivPhoton::leaveRoomReturn() [ルームから退室した結果を処理する]";
			Print << U"- [SivPhoton] errorCode:" << errorCode;
			Print << U"- [SivPhoton] errorString:" << errorString;
		}
	}

	void SivPhoton::joinRandomRoomReturn(const int32 localPlayerID, const int32 errorCode, const String& errorString)
	{
		if (m_verbose)
		{
			Print << U"[SivPhoton] SivPhoton::joinRandomRoomReturn()";
			Print << U"- [SivPhoton] localPlayerID:" << localPlayerID;
			Print << U"- [SivPhoton] errorCode:" << errorCode;
			Print << U"- [SivPhoton] errorString:" << errorString;
		}
	}

	void SivPhoton::joinRoomReturn(const int32 localPlayerID, const int32 errorCode, const String& errorString)
	{
		if (m_verbose)
		{
			Print << U"[SivPhoton] SivPhoton::joinRoomReturn()";
			Print << U"- [SivPhoton] localPlayerID:" << localPlayerID;
			Print << U"- [SivPhoton] errorCode:" << errorCode;
			Print << U"- [SivPhoton] errorString:" << errorString;
		}
	}

	void SivPhoton::joinRoomEventAction(const int32 localPlayerID, const Array<int32>& playerIDs, const bool isSelf)
	{
		if (m_verbose)
		{
			Print << U"[SivPhoton] SivPhoton::joinRoomEventAction() [自分を含め、プレイヤーが参加したら呼ばれる]";
			Print << U"- [SivPhoton] localPlayerID [参加した人の ID]:" << localPlayerID;
			Print << U"- [SivPhoton] playerIDs: [ルームの参加者一覧]" << playerIDs;
			Print << U"- [SivPhoton] isSelf [自分自身の参加？]:" << isSelf;
		}
	}

	void SivPhoton::leaveRoomEventAction(const int32 playerID, const bool isInactive)
	{
		if (m_verbose)
		{
			Print << U"[SivPhoton] SivPhoton::leaveRoomEventAction()";
			Print << U"- [SivPhoton] playerID: " << playerID;
			Print << U"- [SivPhoton] isInactive: " << isInactive;

			if (m_client->getLocalPlayer().getIsMasterClient())
			{
				Print << U"[SivPhoton] I am now the master client";
			}
			else
			{
				Print << U"[SivPhoton] I am still not the master client";
			}
		}
	}

	void SivPhoton::createRoomReturn(const int32 localPlayerID, const int32 errorCode, const String& errorString)
	{
		if (m_verbose)
		{
			Print << U"[SivPhoton] SivPhoton::createRoomReturn() [ルームを新規作成した結果を処理する]";
			Print << U"- [SivPhoton] localPlayerID:" << localPlayerID;
			Print << U"- [SivPhoton] errorCode:" << errorCode;
			Print << U"- [SivPhoton] errorString:" << errorString;
		}
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const int32 data)
	{
		PrintCustomEventAction(U"int32", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const double data)
	{
		PrintCustomEventAction(U"double", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const float data)
	{
		PrintCustomEventAction(U"float", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const bool data)
	{
		PrintCustomEventAction(U"bool", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const String& data)
	{
		PrintCustomEventAction(U"String", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<int32>& data)
	{
		PrintCustomEventAction(U"Array<int32>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<double>& data)
	{
		PrintCustomEventAction(U"Array<double>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<float>& data)
	{
		PrintCustomEventAction(U"Array<float>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<bool>& data)
	{
		PrintCustomEventAction(U"Array<bool>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<String>& data)
	{
		PrintCustomEventAction(U"Array<String>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const ColorF& data)
	{
		PrintCustomEventAction(U"ColorF", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Color& data)
	{
		PrintCustomEventAction(U"Color", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const HSV& data)
	{
		PrintCustomEventAction(U"HSV", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Point& data)
	{
		PrintCustomEventAction(U"Point", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Vec2& data)
	{
		PrintCustomEventAction(U"Vec2", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Vec3& data)
	{
		PrintCustomEventAction(U"Vec3", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Vec4& data)
	{
		PrintCustomEventAction(U"Vec4", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Float2& data)
	{
		PrintCustomEventAction(U"Float2", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Float3& data)
	{
		PrintCustomEventAction(U"Float3", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Float4& data)
	{
		PrintCustomEventAction(U"Float4", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Mat3x2& data)
	{
		PrintCustomEventAction(U"Mat3x2", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Rect& data)
	{
		PrintCustomEventAction(U"Rect", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Circle& data)
	{
		PrintCustomEventAction(U"Circle", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Line& data)
	{
		PrintCustomEventAction(U"Line", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Triangle& data)
	{
		PrintCustomEventAction(U"Triangle", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const RectF& data)
	{
		PrintCustomEventAction(U"RectF", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Quad& data)
	{
		PrintCustomEventAction(U"Quad", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Ellipse& data)
	{
		PrintCustomEventAction(U"Ellipse", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const RoundRect& data)
	{
		PrintCustomEventAction(U"RoundRect", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<ColorF>& data)
	{
		PrintCustomEventAction(U"Array<ColorF>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<Color>& data)
	{
		PrintCustomEventAction(U"Array<Color>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<HSV>& data)
	{
		PrintCustomEventAction(U"Array<HSV>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<Point>& data)
	{
		PrintCustomEventAction(U"Array<Point>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<Vec2>& data)
	{
		PrintCustomEventAction(U"Array<Vec2>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<Vec3>& data)
	{
		PrintCustomEventAction(U"Array<Vec3>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<Vec4>& data)
	{
		PrintCustomEventAction(U"Array<Vec4>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<Float2>& data)
	{
		PrintCustomEventAction(U"Array<Float2>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<Float3>& data)
	{
		PrintCustomEventAction(U"Array<Float3>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(const int32 playerID, const int32 eventCode, const Array<Float4>& data)
	{
		PrintCustomEventAction(U"Array<Float4>", playerID, eventCode, data);
	}

	void SivPhoton::customEventAction(int32 playerID, int32 eventCode, [[maybe_unused]] Deserializer<MemoryReader>& reader)
	{
		Print << U"[SivPhoton] SivPhoton::customEventAction(Deserializer<MemoryReader>)";
		Print << U"[SivPhoton] playerID: " << playerID;
		Print << U"[SivPhoton] eventCode: " << eventCode;
	}

	ExitGames::LoadBalancing::Client& SivPhoton::getClient()
	{
		assert(m_client);

		return *m_client;
	}
}
