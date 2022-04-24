﻿//-----------------------------------------------
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
//	- Luke
//-----------------------------------------------
# pragma once
# include <Siv3D.hpp>

# if SIV3D_PLATFORM(WINDOWS)
#	if SIV3D_BUILD(DEBUG)
#		pragma comment (lib, "Common-cpp/lib/Common-cpp_vc16_debug_windows_mt_x64")
#		pragma comment (lib, "Photon-cpp/lib/Photon-cpp_vc16_debug_windows_mt_x64")
#		pragma comment (lib, "LoadBalancing-cpp/lib/LoadBalancing-cpp_vc16_debug_windows_mt_x64")
#	else
#		pragma comment (lib, "Common-cpp/lib/Common-cpp_vc16_release_windows_mt_x64")
#		pragma comment (lib, "Photon-cpp/lib/Photon-cpp_vc16_release_windows_mt_x64")
#		pragma comment (lib, "LoadBalancing-cpp/lib/LoadBalancing-cpp_vc16_release_windows_mt_x64")
#	endif
# endif

// Photono SDK クラスの前方宣言
namespace ExitGames::LoadBalancing
{
	class Listener;
	class Client;
}

namespace s3d
{
	/// @brief ルーム名
	using RoomName = String;

	/// @brief ルーム名の View
	using RoomNameView = StringView;

	/// @brief ルーム内でのローカル ID を表現する型
	using LocalPlayerID = int32;

	/// @brief ルーム内のローカルプレイヤーの情報
	struct LocalPlayer
	{
		/// @brief ルーム内でのローカル ID
		LocalPlayerID localID = 0;

		/// @brief ユーザ名
		String userName;

		/// @brief ユーザ ID
		String userID;

		/// @brief ルームのホストであるか
		bool isHost = false;

		/// @brief アクティブであるか
		bool isActive = false;
	};

	/// @brief マルチプレイヤー用クラス (Photon バックエンド)
	class Multiplayer_Photon
	{
	public:

		/// @brief デフォルトコンストラクタ
		SIV3D_NODISCARD_CXX20
		Multiplayer_Photon() = default;

		/// @brief マルチプレイヤー用クラスを作成します。
		/// @param secretPhotonAppID Photon アプリケーション ID
		/// @param photonAppVersion アプリケーションのバージョン
		/// @param デバッグ用の Print 出力をする場合 Verbose::Yes, それ以外の場合は Verbose::No
		/// @remark アプリケーションバージョンが異なるプレイヤーとの通信はできません。
		SIV3D_NODISCARD_CXX20
		Multiplayer_Photon(std::string_view secretPhotonAppID, StringView photonAppVersion, Verbose verbose = Verbose::Yes);

		/// @brief デストラクタ
		virtual ~Multiplayer_Photon();

		/// @brief マルチプレイヤー用クラスを作成します。
		/// @param secretPhotonAppID Photon アプリケーション ID
		/// @param photonAppVersion アプリケーションのバージョン
		/// @param verbose デバッグ用の Print 出力をする場合 Verbose::Yes, それ以外の場合は Verbose::No
		/// @remark アプリケーションバージョンが異なるプレイヤーとの通信はできません。
		void init(StringView secretPhotonAppID, StringView photonAppVersion, Verbose verbose = Verbose::Yes);

		/// @brief Photon サーバへの接続を試みます。
		/// @param userName ユーザ名
		/// @param region 接続するサーバのリージョン。unspecified の場合は利用可能なサーバのうち最速のものが選択されます
		/// @remark リージョンは https://doc.photonengine.com/en-us/pun/current/connection-and-authentication/regions を参照してください。
		void connect(StringView userName, const Optional<String>& region = unspecified);

		/// @brief Photon サーバから切断を試みます。
		void disconnect();

		/// @brief サーバーと同期します。
		/// @remark 6 秒間以上この関数を呼ばないと自動的に切断されます。
		void update();

		/// @brief ランダムなルームに参加を試みます。
		/// @param maxPlayers ルームの最大人数
		/// @remark 最大 255, 無料の Photon アカウントの場合は 20
		void joinRandomRoom(int32 maxPlayers);

		/// @brief 指定したルームに参加を試みます。
		/// @param roomName ルーム名
		void joinRoom(RoomNameView roomName);

		/// @brief ルームの作成を試みます。
		/// @param roomName ルーム名
		/// @param maxPlayers ルームの最大人数
		/// @remark 最大 255, 無料の Photon アカウントの場合は 20
		void createRoom(RoomNameView roomName, int32 maxPlayers);

		/// @brief ルームからの退出を試みます。
		void leaveRoom();

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, bool value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, uint8 value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, int16 value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, int32 value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, int64 value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, float value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, double value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const char32* value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, StringView value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const String& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Array<bool>& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Array<uint8>& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Array<int16>&value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Array<int32>& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Array<int64>&value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Array<float>& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Array<double>& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Array<String>& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Color& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const ColorF& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const HSV& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Point& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Vec2& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Vec3& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Vec4& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Float2& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Float3& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Float4& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Mat3x2& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Rect& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Circle& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Line& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Triangle& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const RectF& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Quad& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const Ellipse& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void sendEvent(uint8 eventCode, const RoundRect& value);

		/// @brief データをルームに送信します。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		/// @remark 自作クラスを送信する際に利用できます。
		void sendEvent(uint8 eventCode, const Serializer<MemoryWriter>& writer);

		/// @brief 自身のユーザ名を返します。
		/// @return 自身のユーザ名
		[[nodiscard]]
		String getUserName() const;

		/// @brief 自動生成された自身のユーザ ID を取得します。
		/// @return 自身のユーザ ID
		[[nodiscard]]
		String getUserID() const;

		/// @brief ルーム内でのプレイヤー ID を返します。
		/// @return ルーム内でのプレイヤー ID, ルームに参加していない場合は none
		[[nodiscard]]
		Optional<LocalPlayerID> getLocalPlayerID() const;

		/// @brief 存在するルームの一覧を返します。
		/// @return 存在するルームの一覧
		[[nodiscard]]
		Array<RoomName> getRoomNameList() const;

		/// @brief 自分がロビーにいるかを返します。
		/// @return ロビーにいる場合 true, それ以外の場合は false
		[[nodiscard]]
		bool isInLobby() const;

		/// @brief 自分がロビーまたはルームにいるかを返します。
		/// @return ロビーまたはルームいる場合 true, それ以外の場合は false
		[[nodiscard]]
		bool isInLobbyOrInRoom() const;

		/// @brief 自分がルームに参加しているかを返します。
		/// @return ルームに参加している場合 true, それ以外の場合は false
		[[nodiscard]]
		bool isInRoom() const;

		/// @brief 現在参加しているルーム名を返します。
		/// @return 現在のルーム名。ルームに参加していない場合は空の文字列
		[[nodiscard]]
		String getCurrentRoomName() const;

		/// @brief 現在のルームにいるローカルプレイヤーの情報一覧を返します。
		/// @return 現在のルームにいるローカルプレイヤーの情報一覧
		[[nodiscard]]
		Array<LocalPlayer> getLocalPlayers() const;

		/// @brief 現在のルームに存在するプレイヤーの人数を返します。
		/// @return プレイヤーの人数
		[[nodiscard]]
		int32 getPlayerCountInCurrentRoom() const;

		/// @brief 現在のルームの最大人数を返します。
		/// @return ルームの最大人数
		[[nodiscard]]
		int32 getMaxPlayersInCurrentRoom() const;

		/// @brief 現在のルームに他のプレイヤーが参加できるかを返します。
		/// @return 現在のルームに他のプレイヤーが参加できる場合 true, それ以外の場合は false
		[[nodiscard]]
		bool getIsOpenInCurrentRoom() const;

		/// @brief 現在のルームがロビーから見えるかを返します。
		/// @return ルームが見える場合 true, それ以外の場合は false
		[[nodiscard]]
		bool getIsVisibleInCurrentRoom() const;

		/// @brief 現在のルームに他のプレイヤーが参加できるかを設定します。
		/// @param isOpen 他のプレイヤーが参加できる場合 true, それ以外の場合は false
		void setIsOpenInCurrentRoom(bool isOpen);

		/// @brief 現在のルームがロビーから見えるかを設定します。
		/// @param isVisible ルームを見えるようにする場合 true, それ以外の場合は false
		void setIsVisibleInCurrentRoom(bool isVisible);

		/// @brief ルームの数を返します。
		/// @return ルームの数
		[[nodiscard]]
		int32 getCountGamesRunning() const;

		/// @brief ゲームをプレイ中のプレイヤーの数を返します。
		/// @return ゲームをプレイ中のプレイヤーの数
		[[nodiscard]]
		int32 getCountPlayersIngame() const;

		/// @brief サーバに接続しているプレイヤーの数を返します。
		/// @return サーバに接続しているプレイヤーの数
		[[nodiscard]]
		int32 getCountPlayersOnline() const;

		/// @brief 自分が現在のルームのホストであるかを返します。
		/// @return 自分が現在のルームのホストである場合 true, それ以外の場合は false
		[[nodiscard]]
		bool isHost() const;

		/// @brief update() を呼ぶ必要がある状態であるかを返します。
		/// @return  update() を呼ぶ必要がある状態である場合 true, それ以外の場合は false
		[[nodiscard]]
		bool isActive() const noexcept;

		/// @brief サーバへの接続に失敗したときに呼ばれます。
		/// @param errorCode エラーコード
		virtual void connectionErrorReturn(int32 errorCode);

		/// @brief サーバに接続を試みた結果が通知されるときに呼ばれます。
		/// @param errorCode エラーコード
		/// @param errorString エラー文字列
		/// @param region 接続した地域
		/// @param cluster クラスター
		virtual void connectReturn(int32 errorCode, const String& errorString, const String& region, const String& cluster);

		/// @brief サーバから切断したときに呼ばれます。
		virtual void disconnectReturn();

		/// @brief ルームから退出したときに呼ばれます。
		/// @param errorCode エラーコード
		/// @param errorString エラー文字列
		virtual void leaveRoomReturn(int32 errorCode, const String& errorString);

		/// @brief ランダムルームへの参加を試みた結果が通知されるときに呼ばれます。
		/// @param playerID ルーム内のローカルプレイヤー ID
		/// @param errorCode エラーコード
		/// @param errorString エラー文字列
		virtual void joinRandomRoomReturn(LocalPlayerID playerID, int32 errorCode, const String& errorString);

		/// @brief ルームへの参加を試みた結果が通知されるときに呼ばれます。
		/// @param playerID ルーム内のローカルプレイヤー ID
		/// @param errorCode エラーコード
		/// @param errorString エラー文字列
		virtual void joinRoomReturn(LocalPlayerID playerID, int32 errorCode, const String& errorString);

		/// @brief 誰か（自分を含む）が現在のルームに参加したときに呼ばれます。
		/// @param newPlayer 参加者の情報
		/// @param playerIDs ルーム内のプレイヤー全員のローカルプレイヤー ID
		/// @param isSelf 参加したのが自分である場合 true, それ以外の場合は false
		virtual void joinRoomEventAction(const LocalPlayer& newPlayer, const Array<LocalPlayerID>& playerIDs, bool isSelf);

		/// @brief 現在のルームから誰かが退出したときに呼ばれます。
		/// @param playerID 退出者のローカルプレイヤー ID
		/// @param isInactive 退出者が再参加できる場合 true, それ以外の場合は false
		virtual void leaveRoomEventAction(LocalPlayerID playerID, bool isInactive);

		/// @brief 部屋の作成を試みた結果が通知されるときに呼ばれます。
		/// @param playerID 自身のローカルプレイヤー ID
		/// @param errorCode エラーコード
		/// @param errorString エラー文字列
		virtual void createRoomReturn(LocalPlayerID playerID, int32 errorCode, const String& errorString);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, bool data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, uint8 data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, int16 data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, int32 data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, int64 data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, float data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, double data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const String& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Array<bool>& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Array<uint8>& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Array<int16>& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Array<int32>& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Array<int64>& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Array<float>& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Array<double>& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Array<String>& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Color& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const ColorF& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const HSV& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Point& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Vec2& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Vec3& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Vec4& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Float2& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Float3& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Float4& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Mat3x2& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Rect& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Circle& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Line& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Triangle& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const RectF& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Quad& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const Ellipse& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, const RoundRect& data);

		/// @brief ルームでデータを受信した際に呼ばれます。
		/// @param playerID 送信者のローカルプレイヤー ID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		/// @remark 自作クラスを受信する際に利用できます。
		virtual void customEventAction(LocalPlayerID playerID, uint8 eventCode, Deserializer<MemoryViewReader>& reader);

	protected:

		/// @brief 既存のランダムマッチが見つからなかった時のエラーコード
		/// @remark `joinRandomRoomReturn()` で使います。
		static constexpr int32 NoRandomMatchFound = (0x7FFF - 7);

		/// @brief Verbose モード (Print による詳細なデバッグ出力をする場合 true)
		bool m_verbose = true;

	private:

		class PhotonDetail;

		std::unique_ptr<ExitGames::LoadBalancing::Listener> m_listener;

		std::unique_ptr<ExitGames::LoadBalancing::Client> m_client;

		String m_secretPhotonAppID;

		String m_photonAppVersion;

		Optional<String> m_requestedRegion;

		bool m_isActive = false;
	};
}
