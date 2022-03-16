
# pragma once
# include <Siv3D.hpp>

# if SIV3D_PLATFORM(WINDOWS)
# if SIV3D_BUILD(DEBUG)
#   pragma comment (lib, "Common-cpp/lib/Common-cpp_vc16_debug_windows_mt_x64")
#   pragma comment (lib, "Photon-cpp/lib/Photon-cpp_vc16_debug_windows_mt_x64")
#   pragma comment (lib, "LoadBalancing-cpp/lib/LoadBalancing-cpp_vc16_debug_windows_mt_x64")
# else
#   pragma comment (lib, "Common-cpp/lib/Common-cpp_vc16_release_windows_mt_x64")
#   pragma comment (lib, "Photon-cpp/lib/Photon-cpp_vc16_release_windows_mt_x64")
#   pragma comment (lib, "LoadBalancing-cpp/lib/LoadBalancing-cpp_vc16_release_windows_mt_x64")
# endif
# endif

// Photono SDK クラスの前方宣言
namespace ExitGames
{
	namespace LoadBalancing
	{
		class Listener;
		class Client;
	}
}

namespace s3d
{
	namespace NetworkSystem
	{
		/// @brief 暗号化された Photon アプリケーション ID を復号します。
		/// @param encryptedPhotonAppID 暗号化された Photon アプリケーション ID
		/// @return 復号された Photon アプリケーション ID
		inline String DecryptPhotonAppID(StringView encryptedPhotonAppID)
		{
			// [Siv3D ToDo] あとで実装
			return String{ encryptedPhotonAppID };
		}

		inline constexpr int32 NoRandomMatchFound = (0x7FFF - 7);
	}

	/// @brief マルチプレイヤー用クラス (Photon バックエンド)
	class Multiplayer_Photon
	{
	public:

		SIV3D_NODISCARD_CXX20
		Multiplayer_Photon() = default;

		/// @brief マルチプレイヤー用クラスを作成します。
		/// @param secretPhotonAppID Photon アプリケーション ID
		/// @param photonAppVersion アプリケーションのバージョンです。
		/// @param verbose trueの時にデバッグ用の文字列を表示します。
		/// @remark アプリケーションバージョンが異なる Multiplayer_Photon とは通信できません。
		SIV3D_NODISCARD_CXX20
		Multiplayer_Photon(StringView secretPhotonAppID, StringView photonAppVersion, bool verbose = true);

		virtual ~Multiplayer_Photon();

		/// @brief マルチプレイヤー用クラスを作成します。
		/// @param secretPhotonAppID Photon アプリケーション ID
		/// @param photonAppVersion アプリケーションのバージョンです。
		/// @param verbose trueの時にデバッグ用の文字列を表示します。
		void init(StringView secretPhotonAppID, StringView photonAppVersion, bool verbose = true);

		/// @brief Photon サーバへの接続を試みます。
		/// @param userName ユーザ名
		void connect(StringView userName = U"", const Optional<String>& defaultRoomName = unspecified);

		/// @brief Photon サーバから切断を試みます。
		void disconnect();

		/// @brief サーバーと同期します。
		/// @remark 6 秒間以上この関数を呼ばないと自動的に切断されます。
		void update();

		/// @brief ランダムルームに入室した際に呼び出されます。
		/// @param maxPlayers ルームの最大人数
		/// @remark 最大 255, 無料の Photon アカウントの場合は 20
		void opJoinRandomRoom(int32 maxPlayers);

		/// @brief ルームに入室した際に呼び出されます。
		/// @param roomName ルーム名
		/// @param rejoin 退出した際にルームに再接続するか
		void opJoinRoom(StringView roomName, bool rejoin = false);

		/// @brief ルームを作成した際に呼び出されます。
		/// @param roomName ルーム名
		/// @param maxPlayers ルームの最大人数
		/// @remark 最大 255, 無料の Photon アカウントの場合は 20
		void opCreateRoom(StringView roomName, int32 maxPlayers);

		/// @brief ルームを退出した際に呼び出されます。
		void opLeaveRoom();

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const ColorF& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Color& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const HSV& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Point& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Vec2& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Vec3& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Vec4& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Float2& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Float3& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Float4& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Mat3x2& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Rect& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Circle& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Line& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Triangle& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const RectF& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Quad& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Ellipse& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const RoundRect& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<ColorF>& values);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<Color>& values);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<HSV>& values);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<Point>& values);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<Vec2>& values);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<Vec3>& values);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<Vec4>& values);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<Float2>& values);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<Float3>& values);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<Float4>& values);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		/// @remark 自作クラスを送信する際に利用できます。
		void opRaiseEvent(uint8 eventCode, Serializer<MemoryWriter>& writer);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, int32 value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, double value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, float value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, bool value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, StringView value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<int32>& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<double>& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<float>& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<bool>& value);

		/// @brief データの送信を行います。
		/// @param eventCode イベントコード
		/// @param value 送信するデータ
		void opRaiseEvent(uint8 eventCode, const Array<String>& value);

		/// @brief サーバに接続したときのユーザ名を返します。
		/// @return ユーザ名
		[[nodiscard]]
		String getUserName() const;

		/// @brief サーバに接続したときのユーザ ID (ユーザ名 + タイムスタンプ）を返します。
		/// @return ユーザ ID
		[[nodiscard]]
		String getUserID() const;

		/// @brief 存在するルーム名の一覧を返します。
		/// @remark 自分でルームを作成してそれに参加すると表示されないっぽい (?)
		/// @return 存在するルーム名の一覧
		[[nodiscard]]
		Array<String> getRoomNameList() const;

		/// @brief 自分がルームに参加しているかを返します。
		/// @return ルームに参加している場合 true, それ以外の場合は false
		[[nodiscard]]
		bool isInRoom() const;

		/// @brief 現在のルーム名を返します。
		/// @return 現在のルーム名
		[[nodiscard]]
		String getCurrentRoomName() const;

		/// @brief 現在のルームに存在するプレイヤーの人数を返します。
		/// @return プレイヤーの人数
		[[nodiscard]]
		int32 getPlayerCountInCurrentRoom() const;

		/// @brief 現在のルームの最大人数を返します。
		/// @return ルームの最大人数
		[[nodiscard]]
		int32 getMaxPlayersInCurrentRoom() const;

		/// @brief 現在のルームに新たに他のプレイヤーが参加できるかを返します。
		/// @return ルームが開いている場合 true, それ以外の場合は false
		[[nodiscard]]
		bool getIsOpenInCurrentRoom() const;

		/// @brief 現在のルームがロビーから見えるかを返します。
		/// @return ルームが見えている場合 true, それ以外の場合は false
		[[nodiscard]]
		bool getIsVisibleInCurrentRoom() const;

		/// @brief  現在のルームに新たに他のプレイヤーが参加できるかを設定します。
		/// @param isOpen 他のプレイヤーが参加できる場合 true, それ以外の場合は false
		void setIsOpenInCurrentRoom(bool isOpen);

		/// @brief 現在のルームがロビーから見せるかを設定します。
		/// @param isVisible ルームを見せる場合 true, それ以外の場合は false
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

		/// @brief ルーム内でのプレイヤー ID を返します。
		/// @return ルーム内でのプレイヤー ID, ルームに参加していない場合は none
		[[nodiscard]]
		Optional<int32> localPlayerID() const;

		/// @brief 自分がマスタークライアントであるかを返します。
		/// @return マスタークライアントである場合 true, それ以外の場合は false
		[[nodiscard]]
		bool isMasterClient() const;

		/// @brief Photon SDKを使用しているかを返します。
		/// @return  Photon SDKを使用している場合 true, それ以外の場合は false
		[[nodiscard]]
		bool isUsePhoton() const noexcept;

		/// @brief サーバへの接続に失敗した事を通知します。
		/// @param errorCode エラーコード
		virtual void connectionErrorReturn(int32 errorCode);

		/// @brief サーバに接続を試みた結果を通知します。
		/// @param errorCode エラーコード
		/// @param errorString エラー文字列
		/// @param region 接続した地域
		/// @param cluster サブ的なregion
		virtual void connectReturn(int32 errorCode, const String& errorString, const String& region, const String& cluster);

		/// @brief サーバから切断した事を通知します。
		virtual void disconnectReturn();

		/// @brief ルームから退出した事を通知します。
		/// @param errorCode エラーコード
		/// @param errorString エラー文字列
		virtual void leaveRoomReturn(int32 errorCode, const String& errorString);

		/// @brief ランダムルームへの入室を試みた結果を通知します。
		/// @param localPlayerID ルーム内のプレイヤーID
		/// @param errorCode エラーコード
		/// @param errorString エラー文字列
		virtual void joinRandomRoomReturn(int32 localPlayerID, int32 errorCode, const String& errorString);

		/// @brief ルームへの入室を試みた結果を通知します。
		/// @param localPlayerID ルーム内のプレイヤーID
		/// @param errorCode エラーコード
		/// @param errorString エラー文字列
		virtual void joinRoomReturn(int32 localPlayerID, int32 errorCode, const String& errorString);

		/// @brief ルームに誰かが(自分も含めて)入室した事を通知します。
		/// @param localPlayerID 入室したプレイヤーのID
		/// @param playerIDs ルーム内のプレイヤー全員のID
		/// @param isSelf 入室したのが自分である場合 true, それ以外の場合は false
		virtual void joinRoomEventAction(int32 localPlayerID, const Array<int32>& playerIDs, bool isSelf);

		/// @brief ルームから誰かが退室した事を通知します。
		/// @param playerID 入室したプレイヤーのID
		/// @param isInactive 再入室できる場合 true, それ以外の場合は false
		virtual void leaveRoomEventAction(int32 playerID, bool isInactive);

		/// @brief 部屋の作成を試みた結果を通知します。
		/// @param localPlayerID プレイヤーID
		/// @param errorCode エラーコード
		/// @param errorString エラー文字列
		virtual void createRoomReturn(int32 localPlayerID, int32 errorCode, const String& errorString);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, int32 data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, double data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, float data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, bool data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const String& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<int32>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<double>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<float>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<bool>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<String>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const ColorF& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Color& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const HSV& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Point& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Vec2& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Vec3& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Vec4& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Float2& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Float3& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Float4& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Mat3x2& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Rect& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Circle& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Line& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Triangle& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const RectF& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Quad& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Ellipse& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const RoundRect& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<ColorF>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<Color>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<HSV>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<Point>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<Vec2>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<Vec3>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<Vec4>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<Float2>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<Float3>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		virtual void customEventAction(int32 playerID, int32 eventCode, const Array<Float4>& data);

		/// @brief データを受信した際に呼び出されます。
		/// @param playerID 送信したプレイヤーのID
		/// @param eventCode イベントコード
		/// @param data 受信したデータ
		/// @remark 自作クラスを受信する際に利用できます。
		virtual void customEventAction(int32 playerID, int32 eventCode, Deserializer<MemoryReader>& reader);

	protected:

		String m_defaultRoomName;

	private:

		class PhotonDetail;

		std::unique_ptr<ExitGames::LoadBalancing::Listener> m_listener;

		std::unique_ptr<ExitGames::LoadBalancing::Client> m_client;

		bool m_isUsePhoton = false;

		// モードが有効なときのみ Print するようにします。
		bool m_verbose = true;

		/// @brief リスナーの参照を返します。
		/// @return リスナーの参照
		[[nodiscard]]
		ExitGames::LoadBalancing::Client& getClient();

		template <class Type>
		void PrintCustomEventAction(StringView type, int32 playerID, int32 eventCode, const Type& data)
		{
			Print << U"[Multiplayer_Photon] Multiplayer_Photon::customEventAction(" << type << U")";
			Print << U"- [Multiplayer_Photon] playerID: " << playerID;
			Print << U"- [Multiplayer_Photon] eventCode: " << eventCode;
			Print << U"- [Multiplayer_Photon] data: " << data;
		}
	};
}
