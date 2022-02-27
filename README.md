# SivPhoton

## 概要
SivPhotonは、OpenSiv3DとPhoton Realtime SDKを組み合わせたライブラリです。このライブラリを使用する事で、オンラインゲーム等のリアルタイム通信アプリを制作する事ができます。また、Siv3Dライクな書き方ができるので簡単に制作する事ができます。

## 制作者

[mak1a](https://twitter.com/mak1a_ctrl/)<br>
[Luke](https://twitter.com/Luke02561/)

## 使用エンジン

[OpenSiv3D](https://siv3d.github.io/ja-jp/) v0.6.x<br>
[Photon Realtime SDK](https://www.photonengine.com/ja/sdks) v5.x

## 開発可能環境

Windows10 64bit<br>
macOS Catalina

## リファレンス
[Lukeさん](https://twitter.com/Luke02561/)が現在制作中のリファレンスを公開しています。途中段階ですが[こちら](https://gist.github.com/Luke256/e48b393e8667d6b0e5b85f0e23fd00d0)から確認する事ができます。是非ご活用ください。

## 使用方法
使用方法につきましては現在制作中です。簡易的ではございますが、SivPhotonについての記事がございますので完成まで[こちら](https://qiita.com/makia/items/5401b2bae218e7e7df1a)をご覧ください。

## サンプルコード
### Hello, SivPhoton!

![Screenshot](https://user-images.githubusercontent.com/42209021/151197222-daa85130-5f58-4748-9843-a3040356d27a.gif)

```cpp:Main.cpp
# include <Siv3D.hpp> // OpenSiv3D v0.6.3
# include "NetworkSystem.hpp"
# include "ENCRYPTED_PHOTON_APP_ID.SECRET"

struct Data
{
	Point pos;
	String s;

	// シリアライズに対応させるためのメンバ関数を定義する
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive)
	{
		archive(pos, s);
	}
};

class MyNetwork : public SivPhoton
{
public:

	using SivPhoton::SivPhoton;

	void connectReturn(const int32 errorCode, const String& errorString, const String& region, const String& cluster) override
	{
		Print << U"MyNetwork::connectReturn() [サーバへの接続に成功したときに(?) 呼ばれる]";
		Print << U"- error: " << errorString;
		Print << U"- region: " << region;
		Print << U"- cluster [サブ的な region]: " << cluster;

		if (errorCode)
		{
			return;
		}

		if (MessageBoxResult::Yes == System::MessageBoxYesNo(U"新しくルームを作りますか？", MessageBoxStyle::Question))
		{
			const String roomName = m_defaultRoomName;

			this->opCreateRoom(roomName, MaxPlayers);
		}
	}

	void connectionErrorReturn(const int32 errorCode) override
	{
		Print << U"MyNetwork::connectionErrorReturn() [サーバへの接続が失敗したときに呼ばれる]";
		Print << U"- errorCode: " << errorCode;
	}

	void joinRandomRoomReturn(const int32 localPlayerID, const int32 errorCode, const String& errorString) override
	{
		Print << U"MyNetwork::joinRandomRoomReturn() [既存のランダムなルームに参加した結果を処理する]";
		Print << U"- localPlayerID [失敗した場合は 0 (?)]: " << localPlayerID;
		Print << U"- errorCode: " << errorCode;
		Print << U"- errorString: " << errorString;

		if (errorCode == NetworkSystem::NoRandomMatchFound) // 100% こうなるはず
		{
			Print << U"[既存のルームが見つからなかった]";

			const String roomName = m_defaultRoomName;

			// 自分でルームを新規作成する
			this->opCreateRoom(roomName, MaxPlayers);
		}
	}

	void joinRoomReturn(const int32 localPlayerID, const int32 errorCode, const String& errorString) override
	{
		Print << U"MyNetwork::joinRoomReturn() [指定したルームに参加した結果を処理する]";
		Print << U"- localPlayerID [失敗した場合は 0 (?)]: " << localPlayerID;
		Print << U"- errorCode: " << errorCode;
		Print << U"- errorString: " << errorString;

		if (errorCode)
		{
			Print << U"[指定したルームに参加できなかった(passwordが間違っています)]";
			return;
		}

		Print << U"[指定したルームに参加できた]";
	}

	/// @brief 受信のコード(サンプル)
	/// @param playerID 送信したプレイヤーのID
	/// @param eventCode イベントコード(番号によって意味を持たせたい場合有用)
	/// @param eventContent 受信した本体
	void customEventAction(const int32 playerID, const int32 eventCode, const double eventContent) override
	{
		Print << U"MyNetwork::customEventAction(double)";
		Print << U"- playerID: " << playerID;
		Print << U"- eventCode: " << eventCode;
		Print << U"- eventContent: " << eventContent;
	}

	/// @brief 受信のコード(サンプル)
	/// @param playerID 送信したプレイヤーのID
	/// @param eventCode イベントコード(番号によって意味を持たせたい場合有用)
	/// @param eventContent 受信した本体
	void customEventAction(const int32 playerID, const int32 eventCode, Deserializer<MemoryReader>& eventContent) override
	{
		Print << U"MyNetwork::customEventAction(Deserializer<MemoryReader>)";
		Print << U"- playerID: " << playerID;
		Print << U"- eventCode: " << eventCode;
		Array<Data> data;
		eventContent(data);

		for (const auto& v : data)
		{
			Print << U"- eventContent: " << v.pos << U", " << v.s;
		}
	}

private:

	static constexpr int32 MaxPlayers = 2;
};

void Main()
{
	Window::Resize(1280, 720);

	const std::string encryptedAppID{ SIV3D_OBFUSCATE(ENCRYPTED_PHOTON_APP_ID) };
	const String appID = Unicode::WidenAscii(encryptedAppID);

	MyNetwork network{ appID, U"1.0" };

	network.connect(U"Siv");

	const Font font{ 18 };

	while (System::Update())
	{
		font(U"getRoomNameList: {}"_fmt(network.getRoomNameList())).draw(520, 270);
		font(U"getName: {}"_fmt(network.getUserName())).draw(520, 300);
		font(U"getUserID: {}"_fmt(network.getUserID())).draw(520, 330);
		font(U"isInRoom: {}"_fmt(network.isInRoom())).draw(520, 360);
		font(U"getCurrentRoomName: {}"_fmt(network.getCurrentRoomName())).draw(520, 390);

		font(U"getPlayerCountInCurrentRoom: {}"_fmt(network.getPlayerCountInCurrentRoom())).draw(520, 420);
		font(U"getMaxPlayersInCurrentRoom: {}"_fmt(network.getMaxPlayersInCurrentRoom())).draw(520, 450);
		font(U"getIsOpenInCurrentRoom: {}"_fmt(network.getIsOpenInCurrentRoom())).draw(520, 480);

		font(U"localPlayerID: {}"_fmt(network.localPlayerID())).draw(520, 510);
		font(U"isMasterClient: {}"_fmt(network.isMasterClient())).draw(520, 540);

		if (network.isMasterClient())
		{
			Scene::SetBackground(Palette::Skyblue);
		}
		else
		{
			Scene::SetBackground(Palette::DefaultBackground);
		}

		if (SimpleGUI::Button(U"Raise Event double", Vec2{ 800, 20 }))
		{
			network.opRaiseEvent(33, 4.4);
		}
		if (SimpleGUI::Button(U"Raise Event Data", Vec2{ 800, 70 }))
		{
			Serializer<MemoryWriter> writer;
			const Array<Data> data = { {Point{11,22}, U"Siv3D"}, {Point{33, 44}, U"mak1a"} };
			writer(data);
			network.opRaiseEvent(34, writer);
		}

		if (not network.isInRoom())
		{
			const Array<String> roomList = network.getRoomNameList();

			for (auto [i, roomName] : Indexed(roomList))
			{
				if (SimpleGUI::Button(roomName, Vec2{ 600, 20 + i * 40 }))
				{
					network.opJoinRoom(roomName);
				}
			}
		}

		if (SimpleGUI::Button(U"Leave Room", Vec2{ 600, 180 }))
		{
			network.opLeaveRoom();
		}

		if (SimpleGUI::Button(U"Disconnect", Vec2{ 600, 220 }))
		{
			network.disconnect();
		}

		network.update();
	}
}
```

### 三目並べ

![Screenshot](https://user-images.githubusercontent.com/42209021/151191561-ffbd12a8-9a26-44bf-99b1-b2466b46cfed.gif)

```cpp:Main.cpp
# include <Siv3D.hpp>
# include "NetworkSystem.hpp"
# include "ENCRYPTED_PHOTON_APP_ID.SECRET"


namespace CellState {
	constexpr uint8 None = 0;
	constexpr uint8 Maru = 1;
	constexpr uint8 Batsu = 2;
}

class MyNetwork : public SivPhoton
{
public:

	/// @brief SivPhoton を作成します。
	/// @param secretPhotonAppID Photon アプリケーション ID
	/// @param photonAppVersion アプリケーションのバージョンです。
	/// @remark アプリケーションバージョンが異なる SivPhoton とは通信できません。
	MyNetwork(StringView secretPhotonAppID, StringView photonAppVersion)
		: SivPhoton{ secretPhotonAppID, photonAppVersion }
		, m_isInGame{ false }
		, m_turn{ Turn::Self }
		, m_myCellType(CellState::Maru)
		, m_ban(Arg::center(Scene::Center()), MasuSize * 3) {
		Array<Cell> cells;

		for (int y : step(3)) {
			for (int x : step(3)) {
				cells.emplace_back(m_ban.tl().x + x * MasuSize, m_ban.tl().y + y * MasuSize, MasuSize, Point(x, y));
			}
		}

		m_cells = Grid<Cell>(3, 3, cells);
	}

	void update()
	{
		SivPhoton::update();

		if (not m_isInGame)
		{
			if (not isInRoom())
			{
				const Array<String> roomList = getRoomNameList();

				for (auto [i, roomName] : Indexed(roomList))
				{
					if (SimpleGUI::Button(roomName, Vec2{ 600, 20 + i * 40 }))
					{
						opJoinRoom(roomName);
					}
				}
			}

			return;
		}

		if (m_turn == Turn::Enemy)
		{
			return;
		}

		if (m_turn == Turn::Self)
		{
			// マスをクリックしたら手番によって変更する
			for (auto& cell : m_cells) {
				// マスの状態が「None」ではなかったら早期リターン
				// for文の中の処理なのでreturnではなくcontinue
				if (!cell.isStateNone()) {
					continue;
				}

				// マスをクリックしたら、状態を変更する
				// 手番も変更する
				if (cell.isLeftClicked()) {
					cell.changeState(m_myCellType);
					SendOpponent(cell);
					m_turn = Turn::Enemy;
					checkGameEnd();
					return;
				}
			}

			return;
		}

		if (not this->isMasterClient()) {
			return;
		}

		// ボタンを押したか
		if (SimpleGUI::Button(U"Retry", Scene::CenterF().movedBy(-100, 250), 200)) {
			this->opRaiseEvent(0, true);

			// マスの情報の配列を全て「None」に変更する
			for (auto& cell : m_cells) {
				cell.changeState(CellState::None);
			}
			// 手番を先攻に戻す
			m_turn = Turn::Self;

			return;
		}
	}

	void draw() const
	{
		for (const auto& cell : m_cells) {
			cell.draw(Palette::White).drawFrame(5, Palette::Black);
			if (cell.isMouseOver()) {
				cell.draw(ColorF(Palette::Yellow, 0.5));
			}

			cell.DrawPattern(MasuSize * 0.4, 5);
		}

		// 縦3行を調べる
		// trueが返ったら赤線を引く
		for (int y : step(3)) {
			if (checkLine(m_cells[y][0], m_cells[y][1], m_cells[y][2])) {
				Line(m_cells[y][0].center(), m_cells[y][2].center())
					.stretched(30).draw(4, ColorF(Palette::Red, 0.6));
			}
		}

		// 横3列を調べる
		// trueが返ったら赤線を引く
		for (int x : step(3)) {
			if (checkLine(m_cells[0][x], m_cells[1][x], m_cells[2][x])) {
				Line(m_cells[0][x].center(), m_cells[2][x].center())
					.stretched(30).draw(4, ColorF(Palette::Red, 0.6));
			}
		}

		// 斜め(左上から右下)
		// trueが返ったら赤線を引く
		if (checkLine(m_cells[0][0], m_cells[1][1], m_cells[2][2])) {
			Line(m_cells[0][0].center(), m_cells[2][2].center())
				.stretched(30).draw(4, ColorF(Palette::Red, 0.6));
		}

		// 斜め(右上から左下)
		// trueが返ったら赤線を引く
		if (checkLine(m_cells[2][0], m_cells[1][1], m_cells[0][2])) {
			Line(m_cells[2][0].center(), m_cells[0][2].center())
				.stretched(30).draw(4, ColorF(Palette::Red, 0.6));
		}
	}

private:

	class Cell {
	public:
		Cell() noexcept = default;

		constexpr Cell(const double x, const double y, const double size, const Point& coodinate)
			: m_cell{ x, y, size }
			, m_state{ CellState::None }
			, m_coodinate{ coodinate }
		{}

		bool isMouseOver() const {
			return m_cell.mouseOver();
		}


		bool isLeftClicked() const {
			return m_cell.leftClicked();
		}

		constexpr Vec2 center() const {
			return m_cell.center();
		}

		constexpr Point coodinate() const {
			return m_coodinate;
		}

		constexpr uint8 state() const {
			return m_state;
		}
		void changeState(const uint8 state) {
			m_state = state;
		}

		constexpr bool isStateNone() const {
			return m_state == CellState::None;
		}

		const Cell &draw(const ColorF& color) const {
			m_cell.draw(color);
			return *this;
		}

		const Cell& drawFrame(const double thickness, const ColorF& color) const {
			m_cell.drawFrame(thickness, color);
			return *this;
		}

		void DrawPattern(const double size, const double thickness) const {
			if (m_state == CellState::Maru)
			{
				Circle(m_cell.center(), size).drawFrame(thickness, Palette::Black);
				return;
			}

			if (m_state == CellState::Batsu)
			{
				Shape2D::Cross(size, thickness, m_cell.center()).draw(Palette::Black);
				return;
			}
		}

	private:
		RectF m_cell;

		uint8 m_state;

		Point m_coodinate;
	};

	void connectReturn(const int32 errorCode, const String& errorString, const String& region, const String& cluster) override
	{
		if (errorCode)
		{
			return;
		}

		if (MessageBoxResult::Yes == System::MessageBoxYesNo(U"新しくルームを作りますか？", MessageBoxStyle::Question))
		{
			const String roomName = m_defaultRoomName;

			this->opCreateRoom(roomName, MaxPlayers);
		}
	}

	void connectionErrorReturn(const int32 errorCode) override
	{
		Print << U"MyNetwork::connectionErrorReturn() [サーバへの接続が失敗したときに呼ばれる]";
		Print << U"errorCode: " << errorCode;
	}

	void joinRoomReturn(const int32 localPlayerID, const int32 errorCode, const String& errorString) override
	{
		if (errorCode)
		{
			Print << U"[指定したルームに参加できなかった(passwordが間違っています)]";
			return;
		}

		m_turn = Turn::Enemy;
	}

	/// @brief ルームに誰かが(自分も含めて)入室した事を通知します。
	/// @param localPlayerID 入室したプレイヤーのID
	/// @param playerIDs ルーム内のプレイヤー全員のID
	/// @param isSelf 入室したのが自分である場合 true, それ以外の場合は false
	void joinRoomEventAction(const int32 localPlayerID, const Array<int32>& playerIDs, const bool isSelf) override
	{
		m_turn = this->isMasterClient() ? Turn::Self : Turn::Enemy;
		m_myCellType = m_turn == Turn::Self ? CellState::Maru : CellState::Batsu;
		m_isInGame = true;
	}

	/// @brief 受信のコード(サンプル)
	/// @param playerID 送信したプレイヤーのID
	/// @param eventCode イベントコード(マスの状態)
	/// @param data 受信した本体(マスの座標)
	/// @remark 敵が置いたマスの情報を受信する
	void customEventAction(const int32 playerID, const int32 eventCode, const Point& data) override
	{
		m_cells[data].changeState(eventCode);
		m_turn = Turn::Self;
		checkGameEnd();
	}

	/// @brief 受信のコード(サンプル)
	/// @param playerID 送信したプレイヤーのID
	/// @param eventCode イベントコード(番号によって意味を持たせたい場合有用)
	/// @param data 受信した本体
	/// @remark マスの情報の配列を全て「None」に変更する
	void customEventAction(const int32 playerID, const int32 eventCode, const bool data) override
	{
		for (auto& cell : m_cells) {
			cell.changeState(CellState::None);
		}
		// 手番を戻す
		m_turn = Turn::Enemy;
	}

	constexpr bool checkLine(const Cell& state1, const Cell& state2, const Cell& state3) const {
		if (state1.isStateNone() || state2.isStateNone() || state3.isStateNone()) {
			return false;
		}

		return (state1.state() == state2.state() && state2.state() == state3.state());
	}

	void checkGameEnd() {
		if (std::all_of(m_cells.begin(), m_cells.end(), [](const Cell& cell) {return !cell.isStateNone(); })) {
			m_turn = Turn::Result;
			return;
		}

		// 縦3行を調べる
		// trueが返ったら赤線を引く
		for (int y : step(3)) {
			if (checkLine(m_cells[y][0], m_cells[y][1], m_cells[y][2])) {
				m_turn = Turn::Result;
				return;
			}
		}

		// 横3列を調べる
		// trueが返ったら赤線を引く
		for (int x : step(3)) {
			if (checkLine(m_cells[0][x], m_cells[1][x], m_cells[2][x])) {
				m_turn = Turn::Result;
				return;
			}
		}

		// 斜め(左上から右下)
		// trueが返ったら赤線を引く
		if (checkLine(m_cells[0][0], m_cells[1][1], m_cells[2][2])) {
			m_turn = Turn::Result;
			return;
		}

		// 斜め(右上から左下)
		// trueが返ったら赤線を引く
		if (checkLine(m_cells[2][0], m_cells[1][1], m_cells[0][2])) {
			m_turn = Turn::Result;
			return;
		}
	}

	/// @brief 相手に置いたマス目の情報を送る
	/// @param cell 置いたマス目
	void SendOpponent(const Cell& cell) {
		this->opRaiseEvent(cell.state(), cell.coodinate());
	}

	enum class Turn
	{
		Self,
		Enemy,
		Result
	} m_turn;

	static constexpr int32 MaxPlayers = 2;

	// ゲーム中か
	bool m_isInGame = false;

	// 1マスのサイズ
	static constexpr int32 MasuSize = 150;

	// マス目を表示する際の場所の指標
	const RectF m_ban;

	Grid<Cell> m_cells;

	uint8 m_myCellType;
};

void Main()
{
	// ウィンドウを 1280x720 にリサイズ
	Window::Resize(1280, 720);

	// 背景色を設定
	Scene::SetBackground(ColorF{ 0.2, 0.6, 1.0 });

	const std::string encryptedAppID{ SIV3D_OBFUSCATE(ENCRYPTED_PHOTON_APP_ID) };
	const String appID = Unicode::WidenAscii(encryptedAppID);

	MyNetwork network{ appID, U"1.0" };

	network.connect(U"Siv");

	while (System::Update())
	{
		network.update();
		network.draw();
	}
}

```

### 神経衰弱

![Screenshot](https://user-images.githubusercontent.com/42209021/151538598-c4a0293a-5adc-4010-8e0a-e2a1dcc3bbea.gif)

```cpp:Main.cpp
# include <Siv3D.hpp>
# include "NetworkSystem.hpp"
# include "ENCRYPTED_PHOTON_APP_ID.SECRET"


class MyNetwork : public SivPhoton
{
public:

	MyNetwork(StringView secretPhotonAppID, StringView photonAppVersion)
		: SivPhoton{ secretPhotonAppID, photonAppVersion }
		, m_isInGame{ false }
		, m_pack(75, Palette::Red)
		, m_flipCount(0)
		, m_score(0)
		, m_turn(Turn::Player)
		, m_flipNums{}
	{}

	void update()
	{
		SivPhoton::update();

		if (not m_isInGame)
		{
			if (not isInRoom())
			{
				const Array<String> roomList = getRoomNameList();

				for (auto [i, roomName] : Indexed(roomList))
				{
					if (SimpleGUI::Button(roomName, Vec2{ 600, 130 + i * 40 }))
					{
						opJoinRoom(roomName);
					}
				}
			}

			return;
		}

		if (!MouseL.down() || m_turn == Turn::Enemy)
		{
			return;
		}

		if (m_flipCount < 2)
		{
			// カードのクリック判定＆処理
			for (int32 i{}; auto & card : m_cards)
			{
				if (m_pack.regionAt(card.pos, card.angle).leftClicked() && !card.card.isFaceSide && m_flipCount < 2)
				{
					card.card.flip();

					m_flipNums[m_flipCount] = card.card.rank;

					++m_flipCount;

					this->opRaiseEvent(0, i);
					return;
				}
				++i;
			}
			return;
		}

		// めくったカードの数字が一致しているか
		if (m_flipNums[0] == m_flipNums[1])
		{
			auto itr = std::remove_if(m_cards.begin(), m_cards.end(), [](const auto x) {return x.card.isFaceSide; });

			m_cards.erase(itr, m_cards.end());
		}
		else
		{
			// 裏に戻す
			for (auto& x : m_cards)
			{
				x.card.isFaceSide = false;
			}
			m_turn = Turn::Enemy;
		}
		m_flipCount = 0;
		++m_score;

		this->opRaiseEvent(0, true);
	}

	void draw() const
	{
		if (not m_isInGame)
		{
			FontAsset(U"Menu")(U"対戦相手を探しています...").drawAt(Scene::CenterF().x, 40);
			return;
		}

		for (const auto& card : m_cards)
		{
			m_pack(card.card).drawAt(card.pos, card.angle);
		}
		const String turn = (m_turn == Turn::Player) ? U"あなたのターンです : " : U"相手のターンです : ";
		FontAsset(U"Menu")(turn, m_score).drawAt(Scene::CenterF().x, 40);
	}

private:
	struct MyCard
	{
		Vec2 pos;
		double angle;
		PlayingCard::Card card;

		MyCard(const Vec2& pos_, const double angle_, const PlayingCard::Card& card_) noexcept
			: pos(pos_)
			, angle(angle_)
			, card(card_)
		{}
	};

	void connectReturn(const int32 errorCode, const String& errorString, const String& region, const String& cluster) override
	{
		if (errorCode)
		{
			return;
		}

		if (MessageBoxResult::Yes == System::MessageBoxYesNo(U"新しくルームを作りますか？", MessageBoxStyle::Question))
		{
			const String roomName = m_defaultRoomName;

			this->opCreateRoom(roomName, MaxPlayers);
		}
	}

	void connectionErrorReturn(const int32 errorCode) override
	{
		Print << U"MyNetwork::connectionErrorReturn() [サーバへの接続が失敗したときに呼ばれる]";
		Print << U"errorCode: " << errorCode;
	}

	void joinRoomReturn(const int32 localPlayerID, const int32 errorCode, const String& errorString) override
	{
		if (errorCode)
		{
			Print << U"[指定したルームに参加できなかった(passwordが間違っています)]";
			return;
		}

		m_turn = Turn::Enemy;
	}

	/// @brief 誰かが入室した際に呼び出されます。
	/// @param localPlayerID 入室したプレイヤーのID
	/// @param playerIDs プレイヤーのID
	/// @param isSelf 入室したプレイヤーが自分自身の場合 true, そうでない場合 false
	/// @remark ゲームの初期化を行う(カードを配る)
	void joinRoomEventAction(const int32 localPlayerID, const Array<int32>& playerIDs, const bool isSelf) override
	{
		if (this->isMasterClient() && isSelf)
		{
			return;
		}

		m_turn = this->isMasterClient() ? Turn::Player : Turn::Enemy;
		m_isInGame = true;

		Array<std::pair<PlayingCard::Card, int32>> cards;
		{
			auto deck = PlayingCard::CreateDeck(0, false);
			for (int32 i{}; const auto & card : deck)
			{
				cards.emplace_back(card, i);
				++i;
			}
		}

		cards.shuffle();

		{
			for (auto i : step(cards.size()))
			{
				m_cards.emplace_back(Vec2(100 + i % 13 * (m_pack.width() * 1.2), 180 + ((i / 13)) * (m_pack.height() * 1.2))
					, Random(-13_deg, 13_deg)
					, cards[i].first);
			}
		}

		if (this->isMasterClient())
		{
			Array<int32> orders;
			for (uint8 i{}; const auto & [card, order] : cards)
			{
				orders.push_back(order);
				++i;
			}
			this->opRaiseEvent(0, orders);
		}

		ClearPrint();
	}

	/// @brief 自分がマスタークライアントでない場合のカードの初期化を行います。
	/// @param playerID 送信したプレイヤーのID
	/// @param eventCode イベントコード
	/// @param data 受信したデータ
	void customEventAction(const int32 playerID, const int32 eventCode, const Array<int32>& data) override
	{
		for (const auto cards = PlayingCard::CreateDeck(0, false); size_t i : step(cards.size()))
		{
			m_cards[i].card = cards[data[i]];
		}
	}

	/// @brief 相手のターンの際、カードを相手がめくった際に同じ場所をめくります。
	/// @param playerID 送信したプレイヤーのID
	/// @param eventCode イベントコード
	/// @param data 受信したデータ
	void customEventAction(const int32 playerID, const int32 eventCode, const int32 data) override
	{
		m_cards[data].card.flip();

		m_flipNums[m_flipCount] = m_cards[data].card.rank;

		++m_flipCount;
	}

	/// @brief 相手のターンが終わった際に呼び出されます。
	/// @param playerID 送信したプレイヤーのID
	/// @param eventCode イベントコード
	/// @param data 受信したデータ
	void customEventAction(const int32 playerID, const int32 eventCode, const bool data) override
	{
		// めくったカードの数字が一致しているか
		if (m_flipNums[0] == m_flipNums[1])
		{
			auto itr = std::remove_if(m_cards.begin(), m_cards.end(), [](const auto x) {return x.card.isFaceSide; });

			m_cards.erase(itr, m_cards.end());
		}
		else
		{
			// 裏に戻す
			for (auto& x : m_cards)
			{
				x.card.isFaceSide = false;
			}
			// プレイヤーのターンにする
			m_turn = Turn::Player;
		}
		m_flipCount = 0;
		++m_score;
	}

	// カードの大きさや描画用の変数
	PlayingCard::Pack m_pack;

	Array<MyCard> m_cards;

	// 表にしたカードのナンバー(2枚まで保持)
	std::array<uint32, 2> m_flipNums;

	// 表にしたカードの数
	uint32 m_flipCount;

	// ターン数
	uint32 m_score;

	enum class Turn
	{
		Player,
		Enemy,
	} m_turn;

	// 部屋に入れる最大人数
	static constexpr int32 MaxPlayers = 2;

	// ゲーム中か
	bool m_isInGame = false;

};

void Main()
{
	// ウィンドウを 1280x720 にリサイズ
	Window::Resize(1280, 720);

	// 背景色を設定
	Scene::SetBackground(ColorF{ 0.2, 0.6, 1.0 });

	FontAsset::Register(U"Menu", 30, Typeface::Regular);

	const std::string encryptedAppID{ SIV3D_OBFUSCATE(ENCRYPTED_PHOTON_APP_ID) };
	const String appID = Unicode::WidenAscii(encryptedAppID);

	MyNetwork network{ appID, U"1.0" };

	network.connect(U"Siv");

	while (System::Update())
	{
		network.update();
		network.draw();
	}
}

```

## その他
もしもSivPhotonについての質問等ございましたら、[mak1a](https://twitter.com/mak1a_ctrl/)までご連絡ください。また、SivPhotonとSiv3DのSceneManagerを組み合わせたライブラリも公開予定ですので、そちらもお待ちください。
