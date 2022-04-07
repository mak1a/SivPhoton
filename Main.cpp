# include <Siv3D.hpp> // OpenSiv3D v0.6.3
# include "Multiplayer_Photon.hpp"
# include "PHOTON_APP_ID.SECRET" // このファイルは外部公開しないよう注意

class MyNetwork : public Multiplayer_Photon
{
public:

	// 作るルームの最大人数
	static constexpr int32 MaxPlayers = 2;

	using Multiplayer_Photon::Multiplayer_Photon;

private:

	// サーバへの接続を試みた結果を処理する関数を実装する
	void connectReturn(const int32 errorCode, [[maybe_unused]] const String& errorString, [[maybe_unused]] const String& region, [[maybe_unused]] const String& cluster) override
	{
		if (m_verbose)
		{
			Print << U"MyNetwork::connectReturn() [サーバへの接続を試みた結果を処理する]";
		}

		if (errorCode) // サーバーへの接続に失敗していたら errorCode が 0 以外の値
		{
			if (m_verbose)
			{
				Print << U"- Error: {}"_fmt(errorCode);
			}

			return;
		}

		if (m_verbose)
		{
			Print << U"[サーバへの接続に成功]";
		}

		// ランダムなルームに参加を試みる
		this->opJoinRandomRoom(MaxPlayers);
	}

	// 既存のランダムなルームに参加を試みた結果を処理する関数を実装する
	void joinRandomRoomReturn([[maybe_unused]] const int32 localPlayerID, const int32 errorCode, [[maybe_unused]] const String& errorString) override
	{
		if (m_verbose)
		{
			Print << U"MyNetwork::joinRandomRoomReturn() [既存のランダムなルームに参加を試みた結果を処理する]";
		}

		// 最初に誰もランダムなルームを作っていないときは、
		// NetworkSystem::NoRandomMatchFound エラーになる
		if (errorCode == NetworkSystem::NoRandomMatchFound)
		{
			if (m_verbose)
			{
				Print << U"[既存のランダムなルームが見つからなかった]";
			}

			// 自分でルームを新規作成する
			this->opCreateRoom(m_defaultRoomName, MaxPlayers);
		}
	}

	// ルームに誰かが(自分または他人)が入室したときに呼び出される関数を実装する
	virtual void joinRoomEventAction(const int32 localPlayerID, const Array<int32>& playerIDs, const bool isSelf) override
	{
		if (m_verbose)
		{
			Print << U"[Multiplayer_Photon] Multiplayer_Photon::joinRoomEventAction() [自分を含め、プレイヤーが参加したら呼ばれる]";
			Print << U"- [Multiplayer_Photon] localPlayerID [参加した人の ID]:" << localPlayerID;
			Print << U"- [Multiplayer_Photon] playerIDs: [ルームの参加者一覧]" << playerIDs;
			Print << U"- [Multiplayer_Photon] isSelf [自分自身の参加？]:" << isSelf;

			if (isSelf)
			{
				Print << U"[ルームに自分が参加した]";
			}
			else
			{
				Print << U"[ルームに誰かが参加した]";
			}
		}
	}
};

void Main()
{
	Window::Resize(1280, 720);

	// Print で詳細なデバッグ出力をするか
	constexpr bool Verbose = true;

	// Photon App ID (ビルドしたバイナリに App ID が直接埋め込まれないよう、SIV3D_OBFUSCATE() でラップする)
	const std::string secretAppID{ SIV3D_OBFUSCATE(PHOTON_APP_ID) };

	// サーバーと通信する為のクラス
	// 引数: Photon App ID、今回作ったアプリケーションのバージョン、デバッグ出力の有無
	MyNetwork network{ secretAppID, U"1.0", Verbose };

	while (System::Update())
	{
		// サーバーと接続している場合、サーバーと同期する
		network.update();

		// サーバーに接続するボタン
		if (SimpleGUI::Button(U"Connect", Vec2{ 1000, 20 }, 160, (not network.isActive())))
		{
			// ユーザ名
			const String userName = U"Siv";

			// サーバーに接続する
			network.connect(userName);
		}

		// サーバーから切断するボタン
		if (SimpleGUI::Button(U"Disconnect", Vec2{ 1000, 60 }, 160, network.isActive()))
		{
			// サーバーから切断する
			network.disconnect();
		}
	}
}
