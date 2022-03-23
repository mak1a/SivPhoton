//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2022 Ryo Suzuki
//	Copyright (c) 2016-2022 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

//-----------------------------------------------
//	Author (OpenSiv3D 実装会)
//	- mak1a
//  - 
//-----------------------------------------------

# define NOMINMAX
# include <LoadBalancing-cpp/inc/Client.h>
# include "Multiplayer_Photon.hpp"


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
	class CustomType_Photon : public ExitGames::Common::CustomType<CustomType_Photon<Type, customTypeIndex>, customTypeIndex>
	{
	public:

		SIV3D_NODISCARD_CXX20
		CustomType_Photon() = default;

		SIV3D_NODISCARD_CXX20
		explicit CustomType_Photon(const Type& value)
			: ExitGames::Common::CustomType<CustomType_Photon<Type, customTypeIndex>, customTypeIndex>{}
			, m_value{ value } {}

		SIV3D_NODISCARD_CXX20
		CustomType_Photon(const CustomType_Photon& toCopy)
			: ExitGames::Common::CustomType<CustomType_Photon<Type, customTypeIndex>, customTypeIndex>{}
			, m_value{ toCopy.m_value } {}

		virtual ~CustomType_Photon() = default;

		CustomType_Photon& operator=(const CustomType_Photon& toCopy)
		{
			m_value = toCopy.m_value;
			return *this;
		}

		void cleanup() {}

		bool compare(const ExitGames::Common::CustomTypeBase& other) const override
		{
			return m_value == ((CustomType_Photon&)other).m_value;
		}

		void duplicate(ExitGames::Common::CustomTypeBase* pRetVal) const override
		{
			*reinterpret_cast<CustomType_Photon*>(pRetVal) = *this;
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

		const Type& getValue() const noexcept
		{
			return m_value;
		}

	private:

		Type m_value{};
	};


	// Color関連
	using PhotonColor = CustomType_Photon<Color, 0>;
	using PhotonColorF = CustomType_Photon<ColorF, 1>;
	using PhotonHSV = CustomType_Photon<HSV, 2>;

	// 座標関連
	using PhotonPoint = CustomType_Photon<Point, 3>;
	using PhotonVec2 = CustomType_Photon<Vec2, 4>;
	using PhotonVec3 = CustomType_Photon<Vec3, 5>;
	using PhotonVec4 = CustomType_Photon<Vec4, 6>;
	using PhotonFloat2 = CustomType_Photon<Float2, 7>;
	using PhotonFloat3 = CustomType_Photon<Float3, 8>;
	using PhotonFloat4 = CustomType_Photon<Float4, 9>;
	using PhotonMat3x2 = CustomType_Photon<Mat3x2, 10>;

	// 図形関連
	using PhotonRect = CustomType_Photon<Rect, 11>;
	using PhotonCircle = CustomType_Photon<Circle, 12>;
	using PhotonLine = CustomType_Photon<Line, 13>;
	using PhotonTriangle = CustomType_Photon<Triangle, 14>;
	using PhotonRectF = CustomType_Photon<RectF, 15>;
	using PhotonQuad = CustomType_Photon<Quad, 16>;
	using PhotonEllipse = CustomType_Photon<Ellipse, 17>;
	using PhotonRoundRect = CustomType_Photon<RoundRect, 18>;


	static void RegisterTypes()
	{
		PhotonColor::registerType();
		PhotonColorF::registerType();
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
	}

	static void UnregisterTypes()
	{
		PhotonColor::unregisterType();
		PhotonColorF::unregisterType();
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
	}
}

namespace s3d
{
	class Multiplayer_Photon::PhotonDetail : public ExitGames::LoadBalancing::Listener
	{
	public:

		explicit PhotonDetail(Multiplayer_Photon& context)
			: m_context{ context }
		{
			m_receiveEventFunctions.emplace(uint8{ 0 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<Color, 0>(playerID, eventCode, data); });
			m_receiveEventFunctions.emplace(uint8{ 1 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object& data) { receivedCustomType<ColorF, 1>(playerID, eventCode, data); });
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
			
			m_receiveArrayEventFunctions.emplace(uint8{ 0 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Color, 0>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 1 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<ColorF, 1>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 2 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<HSV, 2>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 3 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Point, 3>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 4 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Vec2, 4>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 5 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Vec3, 5>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 6 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Vec4, 6>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 7 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Float2, 7>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 8 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Float3, 8>(playerID, eventCode, data); });
			m_receiveArrayEventFunctions.emplace(uint8{ 9 }, [this](const int playerID, const nByte eventCode, const ExitGames::Common::Object* data) { receivedCustomArrayType<Float4, 9>(playerID, eventCode, data); });
		}

		void debugReturn([[maybe_unused]] const int debugLevel, [[maybe_unused]] const ExitGames::Common::JString& string) override
		{

		}

		void connectionErrorReturn(const int errorCode) override
		{
			m_context.connectionErrorReturn(errorCode);
			m_context.m_isActive = false;
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
					case ExitGames::Common::TypeCode::BYTE:
						{
							const auto values = ExitGames::Common::ValueObject<BYTE*>(eventDataContent.getValue(L"values")).getDataCopy();
							const auto length = *(ExitGames::Common::ValueObject<BYTE*>(eventDataContent.getValue(L"values"))).getSizes();

							Deserializer<MemoryReader> reader{ values, length };
							m_context.customEventAction(playerID, eventCode, reader);
							return;
						}
					case ExitGames::Common::TypeCode::INTEGER:
						{
							const auto values = ExitGames::Common::ValueObject<int*>(eventDataContent.getValue(L"values")).getDataCopy();
							const auto length = *(ExitGames::Common::ValueObject<int*>(eventDataContent.getValue(L"values"))).getSizes();

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
							const auto values = ExitGames::Common::ValueObject<double*>(eventDataContent.getValue(L"values")).getDataCopy();
							const auto length = *(ExitGames::Common::ValueObject<double*>(eventDataContent.getValue(L"values"))).getSizes();

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
							const auto values = ExitGames::Common::ValueObject<float*>(eventDataContent.getValue(L"values")).getDataCopy();
							const auto length = *(ExitGames::Common::ValueObject<float*>(eventDataContent.getValue(L"values"))).getSizes();

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
							const auto values = ExitGames::Common::ValueObject<bool*>(eventDataContent.getValue(L"values")).getDataCopy();
							const auto length = *(ExitGames::Common::ValueObject<bool*>(eventDataContent.getValue(L"values"))).getSizes();

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
							const auto values = ExitGames::Common::ValueObject<ExitGames::Common::JString*>(eventDataContent.getValue(L"values")).getDataCopy();
							const auto length = *(ExitGames::Common::ValueObject<ExitGames::Common::JString*>(eventDataContent.getValue(L"values"))).getSizes();

							Array<String> data(length);
							for (size_t i = 0; i < length; ++i)
							{
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
				m_context.m_isActive = false;
			}
		}

		// disconnect() の結果を通知するコールバック
		void disconnectReturn() override
		{
			m_context.disconnectReturn();
			m_context.m_isActive = false;
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

		Multiplayer_Photon& m_context;

		HashTable<uint8, std::function<void(const int, const nByte, const ExitGames::Common::Object&)>> m_receiveEventFunctions;

		HashTable<uint8, std::function<void(const int, const nByte, const ExitGames::Common::Object*)>> m_receiveArrayEventFunctions;

		template <class Type, uint8 N>
		void receivedCustomType(const int playerID, const nByte eventCode, const ExitGames::Common::Object& eventContent)
		{
			const auto value = ExitGames::Common::ValueObject<CustomType_Photon<Type, N>>(eventContent).getDataCopy().getValue();
			m_context.customEventAction(playerID, eventCode, value);
		}

		template <class Type, uint8 N>
		void receivedCustomArrayType(const int playerID, const nByte eventCode, const ExitGames::Common::Object* eventContent)
		{
			const CustomType_Photon<Type, N>* values = ExitGames::Common::ValueObject<CustomType_Photon<Type, N>*>(eventContent).getDataCopy();
			const auto length = *(ExitGames::Common::ValueObject<CustomType_Photon<Type, N>*>(eventContent)).getSizes();

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
			const CustomType_Photon<Type, N>* values = ExitGames::Common::ValueObject<CustomType_Photon<Type, N>*>(eventContent).getDataCopy();
			const auto length = *(ExitGames::Common::ValueObject<CustomType_Photon<Type, N>*>(eventContent)).getSizes();

			Array<Type> data(length);
			for (size_t i = 0; i < length; ++i)
			{
				data[i] = values[i].getValue();
			}

			Deserializer<MemoryReader> reader{ data.data(), data.size() };
			m_context.customEventAction(playerID, eventCode, reader);
		}
	};
}

namespace s3d
{
	Multiplayer_Photon::Multiplayer_Photon(const std::string_view secretPhotonAppID, const StringView photonAppVersion, const bool verbose)
	{
		init(Unicode::WidenAscii(secretPhotonAppID), photonAppVersion, verbose);
	}

	Multiplayer_Photon::~Multiplayer_Photon()
	{
		UnregisterTypes();

		disconnect();
	}

	void Multiplayer_Photon::init(const StringView secretPhotonAppID, const StringView photonAppVersion, const bool verbose)
	{
		if (m_listener) // すでに初期化済みであれば何もしない
		{
			return;
		}

		m_listener = std::make_unique<PhotonDetail>(*this);
		m_client = std::make_unique<ExitGames::LoadBalancing::Client>(*m_listener, detail::ToJString(secretPhotonAppID), detail::ToJString(photonAppVersion));
		m_verbose = verbose;
		m_isActive = false;

		RegisterTypes();
	}

	void Multiplayer_Photon::connect(const StringView userName, const Optional<String>& defaultRoomName)
	{
		m_defaultRoomName = defaultRoomName.value_or(String{ userName });

		const auto userNameJ = detail::ToJString(userName);
		const auto userID = ExitGames::LoadBalancing::AuthenticationValues{}
		.setUserID(userNameJ + GETTIMEMS());

		if (not m_client->connect({ userID, userNameJ }))
		{
			if (m_verbose)
			{
				Print << U"[Multiplayer_Photon] ExitGmae::LoadBalancing::Client::connect() failed.";
			}
			return;
		}

		m_client->fetchServerTimestamp();
		m_isActive = true;
	}

	void Multiplayer_Photon::disconnect()
	{
		m_client->disconnect();
	}

	void Multiplayer_Photon::update()
	{
		m_client->service();
	}

	void Multiplayer_Photon::opJoinRandomRoom(const int32 maxPlayers)
	{
		if (not InRange(maxPlayers, 0, 255))
		{
			return;
		}

		m_client->opJoinRandomRoom({}, static_cast<uint8>(Clamp(maxPlayers, 1, 255)));
	}

	void Multiplayer_Photon::opJoinRoom(const StringView roomName, const bool rejoin)
	{
		const auto roomNameJ = detail::ToJString(roomName);

		m_client->opJoinRoom(roomNameJ, rejoin);
	}

	void Multiplayer_Photon::opCreateRoom(const StringView roomName, const int32 maxPlayers)
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

	void Multiplayer_Photon::opLeaveRoom()
	{
		constexpr bool willComeBack = false;

		m_client->opLeaveRoom(willComeBack);
	}
}

namespace s3d
{
	constexpr bool Reliable = true;

	template <class Type, class PhotonType>
	void OpRaiseEventArray(std::unique_ptr<ExitGames::LoadBalancing::Client>& client, const uint8 eventCode, const Array<Type>& values)
	{
		Array<PhotonType> data;
		data.reserve(values.size());

		for (const auto& v : values)
		{
			data.emplace_back(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		client->opRaiseEvent(Reliable, ev, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const bool value)
	{
		m_client->opRaiseEvent(Reliable, value, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const int32 value)
	{
		m_client->opRaiseEvent(Reliable, value, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const float value)
	{
		m_client->opRaiseEvent(Reliable, value, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const double value)
	{
		m_client->opRaiseEvent(Reliable, value, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const StringView value)
	{
		m_client->opRaiseEvent(Reliable, detail::ToJString(value), eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<bool>& values)
	{
		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", values.data(), static_cast<int16>(values.size()));

		m_client->opRaiseEvent(Reliable, ev, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<int32>& values)
	{
		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", values.data(), static_cast<int16>(values.size()));

		m_client->opRaiseEvent(Reliable, ev, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<float>& values)
	{
		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", values.data(), static_cast<int16>(values.size()));

		m_client->opRaiseEvent(Reliable, ev, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<double>& values)
	{
		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", values.data(), static_cast<int16>(values.size()));

		m_client->opRaiseEvent(Reliable, ev, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<String>& values)
	{
		Array<ExitGames::Common::JString> data;
		for (const auto& v : values)
		{
			data << detail::ToJString(v);
		}

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(Reliable, ev, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Color& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonColor{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const ColorF& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonColorF{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const HSV& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonHSV{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Point& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonPoint{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Vec2& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonVec2{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Vec3& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonVec3{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Vec4& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonVec4{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Float2& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonFloat2{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Float3& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonFloat3{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Float4& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonFloat4{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Mat3x2& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonMat3x2{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Rect& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonRect{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Circle& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonCircle{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Line& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonLine{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Triangle& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonTriangle{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const RectF& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonRectF{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Quad& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonQuad{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Ellipse& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonEllipse{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const RoundRect& value)
	{
		m_client->opRaiseEvent(Reliable, PhotonRoundRect{ value }, eventCode);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<Color>& values)
	{
		OpRaiseEventArray<Color, PhotonColor>(m_client, eventCode, values);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<ColorF>& values)
	{
		OpRaiseEventArray<ColorF, PhotonColorF>(m_client, eventCode, values);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<HSV>& values)
	{
		OpRaiseEventArray<HSV, PhotonHSV>(m_client, eventCode, values);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<Point>& values)
	{
		OpRaiseEventArray<Point, PhotonPoint>(m_client, eventCode, values);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<Vec2>& values)
	{
		OpRaiseEventArray<Vec2, PhotonVec2>(m_client, eventCode, values);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<Vec3>& values)
	{
		OpRaiseEventArray<Vec3, PhotonVec3>(m_client, eventCode, values);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<Vec4>& values)
	{
		OpRaiseEventArray<Vec4, PhotonVec4>(m_client, eventCode, values);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<Float2>& values)
	{
		OpRaiseEventArray<Float2, PhotonFloat2>(m_client, eventCode, values);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<Float3>& values)
	{
		OpRaiseEventArray<Float3, PhotonFloat3>(m_client, eventCode, values);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, const Array<Float4>& values)
	{
		OpRaiseEventArray<Float4, PhotonFloat4>(m_client, eventCode, values);
	}

	void Multiplayer_Photon::opRaiseEvent(const uint8 eventCode, Serializer<MemoryWriter>& writer)
	{
		const auto& blob = writer->getBlob();
		const BYTE* src = static_cast<const BYTE*>(static_cast<const void*>(blob.data()));
		const size_t size = blob.size();
		Array<BYTE> data(src, (src + size));

		ExitGames::Common::Hashtable ev;
		ev.put(L"ArrayType", L"Array");
		ev.put(L"values", data.data(), static_cast<int16>(data.size()));

		m_client->opRaiseEvent(Reliable, ev, eventCode);
	}

	String Multiplayer_Photon::getUserName() const
	{
		return detail::ToString(m_client->getLocalPlayer().getName());
	}

	String Multiplayer_Photon::getUserID() const
	{
		return detail::ToString(m_client->getLocalPlayer().getUserID());
	}

	Array<String> Multiplayer_Photon::getRoomNameList() const
	{
		const auto roomNameList = m_client->getRoomNameList();
		Array<String> results(roomNameList.getSize());

		for (uint32 i = 0; i < roomNameList.getSize(); ++i)
		{
			results[i] = detail::ToString(roomNameList[i]);
		}

		return results;
	}

	bool Multiplayer_Photon::isInRoom() const
	{
		return m_client->getIsInGameRoom();
	}

	String Multiplayer_Photon::getCurrentRoomName() const
	{
		if (not m_client->getIsInGameRoom())
		{
			return{};
		}

		return detail::ToString(m_client->getCurrentlyJoinedRoom().getName());
	}

	int32 Multiplayer_Photon::getPlayerCountInCurrentRoom() const
	{
		if (not m_client->getIsInGameRoom())
		{
			return 0;
		}

		return m_client->getCurrentlyJoinedRoom().getPlayerCount();
	}

	int32 Multiplayer_Photon::getMaxPlayersInCurrentRoom() const
	{
		if (not m_client->getIsInGameRoom())
		{
			return 0;
		}

		return m_client->getCurrentlyJoinedRoom().getMaxPlayers();
	}

	bool Multiplayer_Photon::getIsOpenInCurrentRoom() const
	{
		return m_client->getCurrentlyJoinedRoom().getIsOpen();
	}

	bool Multiplayer_Photon::getIsVisibleInCurrentRoom() const
	{
		return m_client->getCurrentlyJoinedRoom().getIsVisible();
	}

	void Multiplayer_Photon::setIsOpenInCurrentRoom(const bool isOpen)
	{
		m_client->getCurrentlyJoinedRoom().setIsOpen(isOpen);
	}

	void Multiplayer_Photon::setIsVisibleInCurrentRoom(const bool isVisible)
	{
		m_client->getCurrentlyJoinedRoom().setIsVisible(isVisible);
	}

	int32 Multiplayer_Photon::getCountGamesRunning() const
	{
		return m_client->getCountGamesRunning();
	}

	int32 Multiplayer_Photon::getCountPlayersIngame() const
	{
		return m_client->getCountPlayersIngame();
	}

	int32 Multiplayer_Photon::getCountPlayersOnline() const
	{
		return m_client->getCountPlayersOnline();
	}

	Optional<int32> Multiplayer_Photon::getLocalPlayerID() const
	{
		const int32 localPlayerID = m_client->getLocalPlayer().getNumber();

		if (localPlayerID < 0)
		{
			return none;
		}

		return localPlayerID;
	}

	bool Multiplayer_Photon::isMasterClient() const
	{
		return m_client->getLocalPlayer().getIsMasterClient();
	}

	bool Multiplayer_Photon::isActive() const noexcept
	{
		return m_isActive;
	}

	void Multiplayer_Photon::connectionErrorReturn(const int32 errorCode)
	{
		if (m_verbose)
		{
			Print << U"[Multiplayer_Photon] Multiplayer_Photon::connectionErrorReturn() [サーバへの接続が失敗したときに呼ばれる]";
			Print << U"- [Multiplayer_Photon] errorCode: " << errorCode;
		}
	}

	void Multiplayer_Photon::connectReturn([[maybe_unused]] const int32 errorCode, const String& errorString, const String& region, const String& cluster)
	{
		if (m_verbose)
		{
			Print << U"[Multiplayer_Photon] Multiplayer_Photon::connectReturn()";
			Print << U"- [Multiplayer_Photon] error: " << errorString;
			Print << U"- [Multiplayer_Photon] region: " << region;
			Print << U"- [Multiplayer_Photon] cluster: " << cluster;
		}
	}

	void Multiplayer_Photon::disconnectReturn()
	{
		if (m_verbose)
		{
			Print << U"[Multiplayer_Photon] Multiplayer_Photon::disconnectReturn() [サーバから切断されたときに呼ばれる]";
		}
	}

	void Multiplayer_Photon::leaveRoomReturn(const int32 errorCode, const String& errorString)
	{
		if (m_verbose)
		{
			Print << U"[Multiplayer_Photon] Multiplayer_Photon::leaveRoomReturn() [ルームから退室した結果を処理する]";
			Print << U"- [Multiplayer_Photon] errorCode:" << errorCode;
			Print << U"- [Multiplayer_Photon] errorString:" << errorString;
		}
	}

	void Multiplayer_Photon::joinRandomRoomReturn(const int32 localPlayerID, const int32 errorCode, const String& errorString)
	{
		if (m_verbose)
		{
			Print << U"[Multiplayer_Photon] Multiplayer_Photon::joinRandomRoomReturn()";
			Print << U"- [Multiplayer_Photon] localPlayerID:" << localPlayerID;
			Print << U"- [Multiplayer_Photon] errorCode:" << errorCode;
			Print << U"- [Multiplayer_Photon] errorString:" << errorString;
		}
	}

	void Multiplayer_Photon::joinRoomReturn(const int32 localPlayerID, const int32 errorCode, const String& errorString)
	{
		if (m_verbose)
		{
			Print << U"[Multiplayer_Photon] Multiplayer_Photon::joinRoomReturn()";
			Print << U"- [Multiplayer_Photon] localPlayerID:" << localPlayerID;
			Print << U"- [Multiplayer_Photon] errorCode:" << errorCode;
			Print << U"- [Multiplayer_Photon] errorString:" << errorString;
		}
	}

	void Multiplayer_Photon::joinRoomEventAction(const int32 localPlayerID, const Array<int32>& playerIDs, const bool isSelf)
	{
		if (m_verbose)
		{
			Print << U"[Multiplayer_Photon] Multiplayer_Photon::joinRoomEventAction() [自分を含め、プレイヤーが参加したら呼ばれる]";
			Print << U"- [Multiplayer_Photon] localPlayerID [参加した人の ID]:" << localPlayerID;
			Print << U"- [Multiplayer_Photon] playerIDs: [ルームの参加者一覧]" << playerIDs;
			Print << U"- [Multiplayer_Photon] isSelf [自分自身の参加？]:" << isSelf;
		}
	}

	void Multiplayer_Photon::leaveRoomEventAction(const int32 playerID, const bool isInactive)
	{
		if (m_verbose)
		{
			Print << U"[Multiplayer_Photon] Multiplayer_Photon::leaveRoomEventAction()";
			Print << U"- [Multiplayer_Photon] playerID: " << playerID;
			Print << U"- [Multiplayer_Photon] isInactive: " << isInactive;

			if (m_client->getLocalPlayer().getIsMasterClient())
			{
				Print << U"[Multiplayer_Photon] I am now the master client";
			}
			else
			{
				Print << U"[Multiplayer_Photon] I am still not the master client";
			}
		}
	}

	void Multiplayer_Photon::createRoomReturn(const int32 localPlayerID, const int32 errorCode, const String& errorString)
	{
		if (m_verbose)
		{
			Print << U"[Multiplayer_Photon] Multiplayer_Photon::createRoomReturn() [ルームを新規作成した結果を処理する]";
			Print << U"- [Multiplayer_Photon] localPlayerID:" << localPlayerID;
			Print << U"- [Multiplayer_Photon] errorCode:" << errorCode;
			Print << U"- [Multiplayer_Photon] errorString:" << errorString;
		}
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const bool data)
	{
		PrintCustomEventAction(U"bool", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const int32 data)
	{
		PrintCustomEventAction(U"int32", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const float data)
	{
		PrintCustomEventAction(U"float", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const double data)
	{
		PrintCustomEventAction(U"double", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const String& data)
	{
		PrintCustomEventAction(U"String", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<bool>& data)
	{
		PrintCustomEventAction(U"Array<bool>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<int32>& data)
	{
		PrintCustomEventAction(U"Array<int32>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<float>& data)
	{
		PrintCustomEventAction(U"Array<float>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<double>& data)
	{
		PrintCustomEventAction(U"Array<double>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<String>& data)
	{
		PrintCustomEventAction(U"Array<String>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Color& data)
	{
		PrintCustomEventAction(U"Color", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const ColorF& data)
	{
		PrintCustomEventAction(U"ColorF", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const HSV& data)
	{
		PrintCustomEventAction(U"HSV", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Point& data)
	{
		PrintCustomEventAction(U"Point", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Vec2& data)
	{
		PrintCustomEventAction(U"Vec2", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Vec3& data)
	{
		PrintCustomEventAction(U"Vec3", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Vec4& data)
	{
		PrintCustomEventAction(U"Vec4", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Float2& data)
	{
		PrintCustomEventAction(U"Float2", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Float3& data)
	{
		PrintCustomEventAction(U"Float3", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Float4& data)
	{
		PrintCustomEventAction(U"Float4", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Mat3x2& data)
	{
		PrintCustomEventAction(U"Mat3x2", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Rect& data)
	{
		PrintCustomEventAction(U"Rect", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Circle& data)
	{
		PrintCustomEventAction(U"Circle", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Line& data)
	{
		PrintCustomEventAction(U"Line", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Triangle& data)
	{
		PrintCustomEventAction(U"Triangle", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const RectF& data)
	{
		PrintCustomEventAction(U"RectF", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Quad& data)
	{
		PrintCustomEventAction(U"Quad", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Ellipse& data)
	{
		PrintCustomEventAction(U"Ellipse", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const RoundRect& data)
	{
		PrintCustomEventAction(U"RoundRect", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<Color>& data)
	{
		PrintCustomEventAction(U"Array<Color>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<ColorF>& data)
	{
		PrintCustomEventAction(U"Array<ColorF>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<HSV>& data)
	{
		PrintCustomEventAction(U"Array<HSV>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<Point>& data)
	{
		PrintCustomEventAction(U"Array<Point>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<Vec2>& data)
	{
		PrintCustomEventAction(U"Array<Vec2>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<Vec3>& data)
	{
		PrintCustomEventAction(U"Array<Vec3>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<Vec4>& data)
	{
		PrintCustomEventAction(U"Array<Vec4>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<Float2>& data)
	{
		PrintCustomEventAction(U"Array<Float2>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<Float3>& data)
	{
		PrintCustomEventAction(U"Array<Float3>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(const int32 playerID, const int32 eventCode, const Array<Float4>& data)
	{
		PrintCustomEventAction(U"Array<Float4>", playerID, eventCode, data);
	}

	void Multiplayer_Photon::customEventAction(int32 playerID, int32 eventCode, [[maybe_unused]] Deserializer<MemoryReader>& reader)
	{
		Print << U"[Multiplayer_Photon] Multiplayer_Photon::customEventAction(Deserializer<MemoryReader>)";
		Print << U"[Multiplayer_Photon] playerID: " << playerID;
		Print << U"[Multiplayer_Photon] eventCode: " << eventCode;
	}

	ExitGames::LoadBalancing::Client& Multiplayer_Photon::getClient()
	{
		assert(m_client);

		return *m_client;
	}
}
