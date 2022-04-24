﻿# include <Siv3D.hpp> // OpenSiv3D v0.6.4
# include "Multiplayer_Photon.hpp"
# include "PHOTON_APP_ID.SECRET"

// Multiplayer_Photon を継承したクラス
class MyNetwork : public Multiplayer_Photon
{
public:

	// ルームの定員
	static constexpr int32 MaxPlayers = 2;

	// Multiplayer_Photon のコンストラクタを継承
	using Multiplayer_Photon::Multiplayer_Photon;

private:

	Array<LocalPlayer> m_localPlayers;

	// サーバへの接続を試みた結果を処理する関数をオーバーライドしてカスタマイズする
	void connectReturn([[maybe_unused]] const int32 errorCode, const String& errorString, [[maybe_unused]] const String& region, [[maybe_unused]] const String& cluster) override
	{
		if (m_verbose)
		{
			Print << U"MyNetwork::connectReturn() [サーバへの接続を試みた結果を処理する]";
		}

		if (errorCode) // サーバーへの接続に失敗していたら errorCode が 0 以外の値
		{
			if (m_verbose)
			{
				Print << U"[サーバへの接続に失敗] " << errorString;
			}

			return;
		}

		if (m_verbose)
		{
			Print << U"[サーバへの接続に成功]";
			Print << U"[ユーザ名: {}]"_fmt(getUserName());
			Print << U"[ユーザ ID: {}]"_fmt(getUserID());
		}

		// 背景色を青色に
		Scene::SetBackground(ColorF{ 0.4, 0.5, 0.6 });
	}

	// 既存のランダムなルームに参加を試みた結果を処理する関数をオーバーライドしてカスタマイズする
	void joinRandomRoomReturn([[maybe_unused]] const LocalPlayerID playerID, const int32 errorCode, const String& errorString) override
	{
		if (m_verbose)
		{
			Print << U"MyNetwork::joinRandomRoomReturn() [既存のランダムなルームに参加を試みた結果を処理する]";
		}

		// 参加可能なランダムなルームが無い場合、
		// NoRandomMatchFound エラーになる
		if (errorCode == NoRandomMatchFound)
		{
			// 新しく作るルーム名（既存のルーム名とは重複できない）
			const RoomName roomName = (getUserName() + U"'s room-" + ToHex(RandomUint32()));

			if (m_verbose)
			{
				Print << U"[参加可能なランダムなルームが見つからなかった]";
				Print << U"[自分でルーム " << roomName << U" を新規作成する]";
			}

			// 自分でルームの新規作成を試みる
			createRoom(roomName, MaxPlayers);

			return;
		}

		if (errorCode)
		{
			if (m_verbose)
			{
				Print << U"[既存のランダムなルームへの参加でエラーが発生] " << errorString;
			}

			return;
		}

		if (m_verbose)
		{
			Print << U"[既存のランダムなルームに参加できた]";
		}
	}

	// ルームを新規作成した結果を処理する関数をオーバーライドしてカスタマイズする
	void createRoomReturn([[maybe_unused]] const LocalPlayerID playerID, const int32 errorCode, const String& errorString) override
	{
		if (m_verbose)
		{
			Print << U"MyNetwork::createRoomReturn() [ルームを新規作成した結果を処理する]";
		}

		if (errorCode)
		{
			if (m_verbose)
			{
				Print << U"[ルームの新規作成でエラーが発生] " << errorString;
			}

			return;
		}

		if (m_verbose)
		{
			Print << U"[ルーム " << getCurrentRoomName() << U" の作成に成功]";
		}
	}

	// 誰か（自分を含む）が現在のルームに参加したときに呼ばれる関数をオーバーライドしてカスタマイズする
	void joinRoomEventAction(const LocalPlayer& newPlayer, [[maybe_unused]] const Array<LocalPlayerID>& playerIDs, const bool isSelf) override
	{
		if (m_verbose)
		{
			Print << U"MyNetwork::joinRoomEventAction() [誰か（自分を含む）が現在のルームに参加したときに呼ばれる]";
		}

		if (m_verbose)
		{
			// 自分自身の参加である場合 isSelf は true
			Print << U"[{} (ID: {}) がルームに参加した。ローカル ID: {}] {}"_fmt(newPlayer.userName, newPlayer.userID, newPlayer.localID, (isSelf ? U"(自分自身)" : U""));

			// （今回参加した人を含む）現在ルームに参加しているプレイヤーの情報一覧を取得
			m_localPlayers = getLocalPlayers();

			Print << U"現在の " << getCurrentRoomName() << U" のルームメンバー";

			for (const auto& player : m_localPlayers)
			{
				Print << U"- [{}] {} (id: {}) {}"_fmt(player.localID, player.userName, player.userID, player.isHost ? U"(host)" : U"");
			}
		}
	}

	// 現在のルームから誰かが退出したときに呼ばれる関数をオーバーライドしてカスタマイズする
	void leaveRoomEventAction(const LocalPlayerID playerID, [[maybe_unused]] const bool isInactive) override
	{
		if (m_verbose)
		{
			Print << U"MyNetwork::joinRoomEventAction() [誰かがルームから退出したら呼ばれる]";
		}

		if (m_verbose)
		{
			for (const auto& player : m_localPlayers)
			{
				// 退出した人の ID と一致したら
				if (player.localID == playerID)
				{
					Print << U"[{} (ID: {}, ローカル ID: {}) がルームから退出した]"_fmt(player.userName, player.userID, player.localID);
				}
			}

			// 現在ルームに参加しているプレイヤーの情報一覧を更新
			m_localPlayers = getLocalPlayers();

			Print << U"現在の " << getCurrentRoomName() << U" のルームメンバー";

			for (const auto& player : m_localPlayers)
			{
				Print << U"- [{}] {} (ID: {}) {}"_fmt(player.localID, player.userName, player.userID, player.isHost ? U"(host)" : U"");
			}
		}
	}

	// ルームから退出したときに呼ばれる関数をオーバーライドしてカスタマイズする
	void leaveRoomReturn(int32 errorCode, const String& errorString) override
	{
		if (m_verbose)
		{
			Print << U"MyNetwork::leaveRoomReturn() [ルームから退出したときに呼ばれる]";
		}

		// ルームメンバー情報をクリア
		m_localPlayers.clear();

		if (errorCode)
		{
			if (m_verbose)
			{
				Print << U"[ルームからの退出でエラーが発生] " << errorString;
			}

			return;
		}
	}

	// サーバから切断したときに呼ばれる関数をオーバーライドしてカスタマイズする
	void disconnectReturn() override
	{
		if (m_verbose)
		{
			Print << U"MyNetwork::disconnectReturn() [サーバから切断したときに呼ばれる]";
		}

		// ルームメンバー情報をクリア
		m_localPlayers.clear();

		// 背景色をデフォルトの色に
		Scene::SetBackground(Palette::DefaultBackground);
	}
};

void Main()
{
	// ウィンドウを　1280x720 にリサイズ
	Window::Resize(1280, 720);

	// Photon App ID
	// 実行ファイルに App ID が直接埋め込まれないよう、SIV3D_OBFUSCATE() でラップ
	const std::string secretAppID{ SIV3D_OBFUSCATE(PHOTON_APP_ID) };

	// サーバーと通信するためのクラス
	// - Photon App ID
	// - 今回作ったアプリケーションのバージョン（これが異なるプログラムとは通信できない）
	// - Print によるデバッグ出力の有無
	MyNetwork network{ secretAppID, U"1.0", Verbose::Yes };

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

		// ルームに参加するボタン
		if (SimpleGUI::Button(U"Join Room", Vec2{ 1000, 60 }, 160, network.isInLobby()))
		{
			// 既存のランダムなルームに参加を試みる
			network.joinRandomRoom(MyNetwork::MaxPlayers);
		}

		// 現在ルームから退出するボタン
		if (SimpleGUI::Button(U"Leave Room", Vec2{ 1000, 100 }, 160, network.isInRoom()))
		{
			// 現在のルームから退出を試みる
			network.leaveRoom();
		}

		// サーバーから切断するボタン
		if (SimpleGUI::Button(U"Disconnect", Vec2{ 1000, 140 }, 160, network.isActive()))
		{
			// サーバーから切断する（ルームからも強制的に退出し、leaveRoomReturn() は呼ばれない）
			network.disconnect();
		}
	}

	// Multiplayer_Photon のデストラクタで自動的に切断されるため、明示的に呼ぶ必要はない
	// network.disconnect();
}
