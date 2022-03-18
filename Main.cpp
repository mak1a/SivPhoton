# include <Siv3D.hpp> // OpenSiv3D v0.6.3
# include "Multiplayer_Photon.hpp"
# include "ENCRYPTED_PHOTON_APP_ID.SECRET"

class MyNetwork : public Multiplayer_Photon
{
public:

	// 作るルームの最大人数
	static constexpr int32 MaxPlayers = 2;

	using Multiplayer_Photon::Multiplayer_Photon;

	void connectReturn(const int32 errorCode, const String& errorString, const String& region, const String& cluster) override
	{
		Print << U"MyNetwork::connectReturn() [サーバへの接続に成功したときに呼ばれる]";
		Print << U"- error: " << errorString;
		Print << U"- region: " << region;
		Print << U"- cluster [サブ的な region]: " << cluster;

		if (errorCode)
		{
			return;
		}

		if (MessageBoxResult::Yes == System::MessageBoxYesNo(U"新しくルームを作りますか？", MessageBoxStyle::Question))
		{
			this->opJoinRandomRoom(MaxPlayers);
		}
	}

	void joinRandomRoomReturn(const int32 localPlayerID, const int32 errorCode, const String& errorString) override
	{
		Print << U"MyNetwork::joinRandomRoomReturn() [既存のランダムなルームに参加した結果を処理する]";
		Print << U"- localPlayerID: " << localPlayerID;
		Print << U"- errorCode: " << errorCode;
		Print << U"- errorString: " << errorString;

		if (errorCode == NetworkSystem::NoRandomMatchFound)
		{
			Print << U"[既存のルームが見つからなかった]";

			// 今回はルーム名をデフォルトのルーム名に
			const String roomName = m_defaultRoomName;

			// 自分でルームを新規作成する
			this->opCreateRoom(roomName, MaxPlayers);
		}
	}
};

void Main()
{
	Window::Resize(1280, 720);

	const std::string secretAppID{ SIV3D_OBFUSCATE(ENCRYPTED_PHOTON_APP_ID) };

	// Print で詳細なデバッグ出力をするか
	constexpr bool Verbose = true;

	// ユーザ名
	const String userName = U"Siv";

	MyNetwork network{ secretAppID, U"1.0", Verbose };

	while (System::Update())
	{
		network.update();

		if (SimpleGUI::Button(U"Connect", Vec2{ 600, 20 }, unspecified, (not network.isActive())))
		{
			network.connect(userName);
		}

		if (SimpleGUI::Button(U"Disconnect", Vec2{ 600, 60 }))
		{
			network.disconnect();
		}
	}
}
