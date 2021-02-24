#include "GameMain.h"

//----------------------------------------------------------------------
// コンストラクタ
//----------------------------------------------------------------------
GameMain::GameMain(int num)
{
	map_num = num;
	goal = false;
	Init();

	//----------------------------------------------------------------------
	// コントローラーのDeadZoneを呼ぶ
	//----------------------------------------------------------------------
	controller.setLeftThumbDeadZone();
	controller.setRightThumbDeadZone();
	controller.setLeftTriggerDeadZone();
	controller.setRightTriggerDeadZone();

	//----------------------------------------------------------------------
	// 生成
	//----------------------------------------------------------------------
	// MAP
	map = Create<Map>();
	// ブロック
	TextReadCreatBlock();
	// トラップ
	InitTextCreatTrap();
	WriterTextTrap();
	WriterTextDropTrap();

#if ! defined(NDEBUG)
	// MAP
	ReadTextMAP();
#else
#endif

	camera = CameraBox2D(Vec2(map->GetPlayerPosX() + 210, map->GetPlayerPosY() + 500), 1);
}

//----------------------------------------------------------------------
// デストラクタ
//----------------------------------------------------------------------
GameMain::~GameMain()
{
	//全タスクの消去
	Task::All::Clear();
}
//----------------------------------------------------------------------
// 初期化処理
//----------------------------------------------------------------------
void GameMain::Init()
{
	tout_cnt = 0;
	tout_cnt1 = 0;
	tout_add = 0;
	tout_add1 = 0;
}
//----------------------------------------------------------------------
// 更新処理
//----------------------------------------------------------------------
void GameMain::Update()
{
	if (map->GetPlayerPosX() < CAMERA_MIN_X) {
		camera.setPos(Vec2(CAMERA_MIN_X,CAMERA_MIN_Y));
	}
	if (map->GetPlayerPosX() > CAMERA_MAX_X) {
		camera.setPos(Vec2(CAMERA_MAX_X, CAMERA_MIN_Y));
	}
	if (map->GetPlayerPosX() > CAMERA_MIN_X && map->GetPlayerPosX() < CAMERA_MAX_X) {
		camera.setPos(Vec2(map->GetPlayerPosX(), CAMERA_MIN_Y));
	}


#if ! defined(NDEBUG)
	// トラップの種類を変える
	KeyChangeTrapType();

	// Rキーを押したときリスポーン
	if (Input::KeyR.clicked || controller.buttonY.clicked && map->GetPDeath()) {
		Init();
		// ブロックを生成
		TextReadCreatBlock();
		// 初期テキストを読み込む
		InitTextCreatTrap();
		WriterTextTrap();
		WriterTextDropTrap();
	}
	// 左クリックしたときにファイルの中身を書き換え
	if (Input::MouseL.clicked ) {
		// 0　から 6 までの間
		if (type >= TRIANGLE_UP && type < BLOCK ) {
			// タイプが 6 （ドロップトラップ）でないとき
			if (type != TRIANGLE_DROP) {
				WriterText2Trap();
				WriterTextTrap();
			}// 6 の時
			else {
				WriterText2DropTrap();
				WriterTextDropTrap();
			}
			InitWriterTextTrap();
		}
		// typeが９のときブロック作成
		if (type == BLOCK) {
			ChangeTextBlock();
		}
		// トラップ
		TextReadCreatBlock();
		// トラップを生成
		TextReadCreatTrap();
		TextReadCreatDropTrap();
	}
	// 右クリック
	if (Input::MouseR.clicked) {
		DeleteTrap();
		WriterTextTrap();
		WriterTextDropTrap();
		InitWriterTextTrap();
		// トラップ
		TextReadCreatBlock();
		// トラップを生成
		TextReadCreatTrap();
		TextReadCreatDropTrap();
	}
#else
#endif

	//TaskSend と TaskReceive で設定した関数を呼び出す
	TaskSend::All::Update();
	//TaskCall で設定した関数を呼び出す
	TaskCall::All::Update(CallGroup_Update);

	// TriangleOutの範囲に入ったら
	if (map->GetTOutRange() == true) {
		ChangeTrap();
		WriterTextTrap();
		// トラップを生成
		TextReadCreatOutTrap1();
		map->SetTOutRange();
	}
	// ゴールの赤い四角にプレイヤーが触れるとクリア
	if (map->GetPlayer().intersects(GetRectGoal())) {
		goal = true;
	}

}
//----------------------------------------------------------------------
// 描画処理
//----------------------------------------------------------------------
void GameMain::Draw()
{

	const auto t1 = camera.createTransformer();
	TaskCall::All::Update(CallGroup_Draw);

#if ! defined(NDEBUG)
	// b_posにマウスポジションをブロックのサイズで割った値を入れる
	b_pos = Mouse::Pos() / MAP_SIZE;
	t_pos = Mouse::Pos();
	//　カーソル
	switch (type)
	{
	case TRIANGLE_UP:
		Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE, t_pos.x, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y).drawFrame();
		break;
	case TRIANGLE_DOWN:
		Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE, t_pos.x, t_pos.y + MAP_SIZE).drawFrame();
		break;
	case TRIANGLE_RIGHT:
		Triangle(t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE / 2, t_pos.x, t_pos.y + MAP_SIZE, t_pos.x, t_pos.y).drawFrame();
		break;
	case TRIANGLE_LEFT:
		Triangle(t_pos.x, t_pos.y + MAP_SIZE / 2, t_pos.x + MAP_SIZE, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE).drawFrame();
		break;
	case TRIANGLE_OUT:
		Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y + MAP_SIZE / 2, t_pos.x, t_pos.y, t_pos.x + MAP_SIZE, t_pos.y).drawFrame();
		break;
	case TRIANGLE_DROP:
		Triangle(t_pos.x + MAP_SIZE / 2, t_pos.y - MAP_SIZE / 2, t_pos.x + MAP_SIZE, t_pos.y + MAP_SIZE, t_pos.x, t_pos.y + MAP_SIZE).drawFrame();
		break;
	case BLOCK:
		RectF(Mouse::Pos() / MAP_SIZE * MAP_SIZE, MAP_SIZE, MAP_SIZE).drawFrame();
		break;
	default:
		break;
	}
#else
#endif

	// ゴールの描画
	switch (map_num)
	{
	case 0:
		RectF(MAX_W - 100, MAP_SIZE, MAP_SIZE, MAP_SIZE * 3).draw(Color(255, 0, 0));
		break;
	case 1:
		RectF(MAX_W - 100, MAP_SIZE, MAP_SIZE, MAP_SIZE * 3).draw(Color(255, 0, 0));
		break;
	case 2:
		RectF(MAX_W - 100, MAP_SIZE, MAP_SIZE, MAP_SIZE * 3).draw(Color(255, 0, 0));
		break;
	default:
		break;
	}


	camera.draw();
}
//----------------------------------------------------------------------
// 終了処理
//----------------------------------------------------------------------
void GameMain::Exit()
{
}
//----------------------------------------------------------------------
// 取得したい番号と座標の三角形を取得
//----------------------------------------------------------------------
Triangle GameMain::GetTriangle(int num,double x, double y) const
{
	switch (num)
	{
	case TRIANGLE_UP:
		return  Triangle(x + MAP_SIZE / 2, y + MAP_SIZE, x, y, x + MAP_SIZE, y);
		break;
	case TRIANGLE_DOWN:
		return Triangle(x + MAP_SIZE / 2, y, x + MAP_SIZE, y + MAP_SIZE, x, y + MAP_SIZE);
		break;
	case TRIANGLE_RIGHT:
		return Triangle(x + MAP_SIZE, y + MAP_SIZE / 2, x, y + MAP_SIZE, x, y);
		break;
	case TRIANGLE_LEFT:
		return Triangle(x, y + MAP_SIZE / 2, x + MAP_SIZE, y, x + MAP_SIZE, y + MAP_SIZE);
		break;
	case TRIANGLE_OUT:
		return Triangle(x + MAP_SIZE / 2, y + MAP_SIZE / 2, x, y, x + MAP_SIZE, y);
		break;
	case TRIANGLE_DROP:
		return Triangle(x + MAP_SIZE / 2, y - MAP_SIZE / 2, x + MAP_SIZE, y + MAP_SIZE, x, y + MAP_SIZE);
		break;
	case TRIANGLE_OUT_1:
		return Triangle(x + MAP_SIZE / 2, y + MAP_SIZE + MAP_SIZE / 2, x, y, x + MAP_SIZE, y);
		break;
	default:
		break;
	}
	return Triangle();
}
//----------------------------------------------------------------------
// MAPテキストファイルを読み込んでブロックを作る
//----------------------------------------------------------------------
void GameMain::TextReadCreatBlock()
{
	// 高さ800 底　-320
	// 左　０　右　1200
	// 最初にブロックをすべて消す
	Block::Task::All::Clear();
	// MAP.txtファイルを読む
	TextReader reader;
	switch (map_num)
	{
	case 0:
		reader.open(L"Example/txt/MAP.txt");
		break;
	case 1:
		reader.open(L"Example/txt/MAP2.txt");
		break;
	case 2:
		reader.open(L"Example/txt/MAP3.txt");
		break;
	default:
		break;
	}
	wchar ch;
	b_pos.x = 0;
	b_pos.y = 0;
	while (reader.readChar(ch))
	{
		if (b_pos.x == BLOCK_W) {
			b_pos.x = 0;
			b_pos.y += 1;
		}
		if (ch == '1') {
			// ブロック
			Create<Block>(map->GetWorld(), b_pos.x, b_pos.y);
		}
		b_pos.x += 1;
	}
	reader.close();
}
//----------------------------------------------------------------------
// MAPテキストを読み込む
//----------------------------------------------------------------------
void GameMain::ReadTextMAP()
{
	TextReader reader;
	switch (map_num)
	{
	case 0:
		reader.open(L"Example/txt/MAP.txt");
		break;
	case 1:
		reader.open(L"Example/txt/MAP2.txt");
		break;
	case 2:
		reader.open(L"Example/txt/MAP3.txt");
		break;
	default:
		break;
	}
	MAP_text = reader.readAll();
	reader.close();
}
//----------------------------------------------------------------------
// MAPテキストに書き込む
//----------------------------------------------------------------------
void GameMain::WriterTextMAP()
{
	TextWriter writer;
	switch (map_num)
	{
	case 0:
		writer.open(L"Example/txt/MAP.txt");
		break;
	case 1:
		writer.open(L"Example/txt/MAP2.txt");
		break;
	case 2:
		writer.open(L"Example/txt/MAP3.txt");
		break;
	default:
		break;
	}
	writer.writeln(MAP_text);
	writer.close();
}
//----------------------------------------------------------------------
// MAPテキストの中身の書き換え
//----------------------------------------------------------------------
void GameMain::ChangeTextBlock()
{	
	// １だったら
	if (MAP_text[(int)b_pos.y * BLOCK_W + (int)b_pos.x] == '1') {
		MAP_text[(int)b_pos.y * BLOCK_W + (int)b_pos.x] = '0';
	}
	else {// ０だったら
		MAP_text[(int)b_pos.y * BLOCK_W + (int)b_pos.x] = '1';
	}
	// MAPテキストに書きこみ
	WriterTextMAP();
	
}
//----------------------------------------------------------------------
// 最初だけ呼ばれる関数( Init → Trap2 , DropTrap2 )
//----------------------------------------------------------------------
void GameMain::InitTextCreatTrap()
{
	TextReader reader;
	switch (map_num)
	{
	case MAP_ONE:
		reader.open(L"Example/txt/TrapInit.txt");
		break;
	case MAP_TWO:
		reader.open(L"Example/txt/TrapInit2.txt");
		break;
	case MAP_THREE:
		reader.open(L"Example/txt/TrapInit3.txt");
		break;
	default:
		break;
	}
	TextWriter writer(L"Example/txt/Trap2.txt");
	TextWriter writer2(L"Example/txt/DropTrap2.txt");

	int num = 0;
	Vec2 pos;

	while (reader.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		if (num == TRIANGLE_OUT_1) {
			num = TRIANGLE_OUT;
		}
		
		Create<Trap>(map->GetWorld(), pos.x, pos.y, num);

		if (num == TRIANGLE_DROP) {
			writer2.writeln(pos.x);
			writer2.writeln(pos.y);
			writer2.writeln(num);
			continue;
		}

		writer.writeln(pos.x);
		writer.writeln(pos.y);
		writer.writeln(num);
	}
	reader.close();
	writer.close();
	writer2.close();
}
//----------------------------------------------------------------------
// 初期のテキストにTrapテキストを上書き保存( Trap , DropTrap → Init ) 
//----------------------------------------------------------------------
void GameMain::InitWriterTextTrap()
{
	TextWriter writer;
	switch (map_num)
	{
	case MAP_ONE:
		writer.open(L"Example/txt/TrapInit.txt");
		break;
	case MAP_TWO:
		writer.open(L"Example/txt/TrapInit2.txt");
		break;
	case MAP_THREE:
		writer.open(L"Example/txt/TrapInit3.txt");
		break;
	default:
		break;
	}
	TextReader reader1(L"Example/txt/Trap.txt");
	TextReader reader2(L"Example/txt/DropTrap.txt");
	int num = 0;
	Vec2 pos;
	while (reader1.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader1.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader1.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		writer.writeln(pos.x);
		writer.writeln(pos.y);
		writer.writeln(num);
	}
	reader1.close();
	while (reader2.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader2.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader2.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		writer.writeln(pos.x);
		writer.writeln(pos.y);
		writer.writeln(num);
	}
	reader2.close();
	writer.close();
}
//----------------------------------------------------------------------
// トラップテキストを読み込む( Trap )
//----------------------------------------------------------------------
void GameMain::TextReadCreatTrap()
{
	TextReader reader(L"Example/txt/Trap.txt");

	int num = 0;
	Vec2 pos;

	while (reader.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		Create<Trap>(map->GetWorld(), pos.x, pos.y, num);
		
	}
	reader.close();
}
//----------------------------------------------------------------------
// トラップテキストを読み込み飛び出るトラップを作成( Trap )
//----------------------------------------------------------------------
void GameMain::TextReadCreatOutTrap1()
{
	TextReader reader(L"Example/txt/Trap.txt");

	int num = 0;
	Vec2 pos;
	tout_cnt1 = 0;

	while (reader.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		// ファイルの中のトラップを数える
		tout_cnt1++;

		// num が 7(TRIANGLE_OUT_1)の時
		if (num == TRIANGLE_OUT_1) {
			// ファイルに書かれた場所が同じで、これ以上ファイルに 4(TRIANGLE_OUT)がない時
			if (tout_cnt == tout_cnt1 && tout_add != tout_add1) {
				// クリエイトする
				Create<Trap>(map->GetWorld(), pos.x, pos.y, num);
				// クリエイトされた数を数える
				tout_add1++;
			}
		}
	}
	reader.close();
}
//----------------------------------------------------------------------
// トラップテキストに書き込み( Trap2 → Trap )
//----------------------------------------------------------------------
void GameMain::WriterTextTrap()
{
	TextReader reader(L"Example/txt/Trap2.txt");
	TextWriter writer(L"Example/txt/Trap.txt");
	int num = 0;
	Vec2 pos;
	while (reader.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		writer.writeln(pos.x);
		writer.writeln(pos.y);
		writer.writeln(num);
	}
	reader.close();
	writer.close();
}
//----------------------------------------------------------------------
// トラップ2テキストに書き込み( Trap → Trap2 )
//----------------------------------------------------------------------
void GameMain::WriterText2Trap()
{
	TextReader reader(L"Example/txt/Trap.txt");
	TextWriter writer(L"Example/txt/Trap2.txt");
	int num = 0;
	Vec2 pos;
	while (reader.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		writer.writeln(pos.x);
		writer.writeln(pos.y);
		writer.writeln(num);
	}
	reader.close();
	writer.writeln(t_pos.x);
	writer.writeln(t_pos.y);
	writer.writeln(type);

	writer.close();
}
//----------------------------------------------------------------------
// トラップを消す( Trap → Trap2 )( DropTrap → DropTrap2)
//----------------------------------------------------------------------
void GameMain::DeleteTrap()
{
	TextReader reader(L"Example/txt/Trap.txt");
	TextWriter writer(L"Example/txt/Trap2.txt");
	int num = 0;
	Vec2 pos;
	while (reader.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		if (Triangle(GetTriangle(num,pos.x,pos.y)).intersects(GetTriangle(type,t_pos.x,t_pos.y))) {
			if (num == type) {
				continue;
			}
		}
		writer.writeln(pos.x);
		writer.writeln(pos.y);
		writer.writeln(num);
	}
	reader.close();
	writer.close();

	TextReader reader2(L"Example/txt/DropTrap.txt");
	TextWriter writer2(L"Example/txt/DropTrap2.txt");
	while (reader2.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader2.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader2.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		if (Triangle(GetTriangle(num, pos.x, pos.y)).intersects(GetTriangle(type, t_pos.x, t_pos.y))) {
			if (num == type) {
				continue;
			}
		}
		writer2.writeln(pos.x);
		writer2.writeln(pos.y);
		writer2.writeln(num);
	}
	reader2.close();
	writer2.close();
}
//----------------------------------------------------------------------
// トラップをチェンジする( Trap → Trap2 )
//----------------------------------------------------------------------
void GameMain::ChangeTrap()
{
	TextReader reader(L"Example/txt/Trap.txt");
	TextWriter writer(L"Example/txt/Trap2.txt");
	int num = 0;
	int count = 0;
	Vec2 pos;
	while (reader.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		// ファイルの中のトラップを数える
		count++;

		// プレイヤーとTrapOutの範囲の当たり判定
		if (Circle(pos.x + MAP_SIZE / 2, pos.y + MAP_SIZE / 2, MAP_SIZE + 20).intersects(map->GetPlayer())) {
			// num が　４（TRIANGLE_OUT）の時
			if (num == TRIANGLE_OUT) {
				// num の書き換え
				num = TRIANGLE_OUT_1;
				// tout_cnt に何個目か代入
				tout_cnt = count;
				// 書き換えられた数を数える
				tout_add++;
			}
		}

		writer.writeln(pos.x);
		writer.writeln(pos.y);
		writer.writeln(num);
		
	}
	reader.close();
	writer.close();
}
//----------------------------------------------------------------------
// ドロップトラップテキストを読み込む( DropTrap )
//----------------------------------------------------------------------
void GameMain::TextReadCreatDropTrap()
{
	TextReader reader(L"Example/txt/DropTrap.txt");

	int num = 0;
	Vec2 pos;
	int count = 0;

	while (reader.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		Create<Trap>(map->GetWorld(), pos.x, pos.y, num);

		count++;
	}
	reader.close();
}
//----------------------------------------------------------------------
// トラップテキストに書き込み( DropTrap2 → DropTrap )
//----------------------------------------------------------------------
void GameMain::WriterTextDropTrap()
{
	TextReader reader(L"Example/txt/DropTrap2.txt");
	TextWriter writer(L"Example/txt/DropTrap.txt");
	int num = 0;
	Vec2 pos;
	while (reader.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		writer.writeln(pos.x);
		writer.writeln(pos.y);
		writer.writeln(num);

	}
	reader.close();
	writer.close();

}
//----------------------------------------------------------------------
// トラップテキストに書き込み( DropTrap → DropTrap2 )
//----------------------------------------------------------------------
void GameMain::WriterText2DropTrap()
{
	TextReader reader(L"Example/txt/DropTrap.txt");
	TextWriter writer(L"Example/txt/DropTrap2.txt");
	int num = 0;
	Vec2 pos;
	while (reader.readLine(Trap_text))
	{
		pos.x = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		pos.y = Parse<double>(Trap_text);
		reader.readLine(Trap_text);
		num = Parse<int>(Trap_text);

		writer.writeln(pos.x);
		writer.writeln(pos.y);
		writer.writeln(num);
	}
	reader.close();
	writer.writeln(t_pos.x);
	writer.writeln(t_pos.y);
	writer.writeln(type);

	writer.close();
}
//----------------------------------------------------------------------
// トラップの種類をキーで切り替え
//----------------------------------------------------------------------
void GameMain::KeyChangeTrapType()
{
	if (controller.buttonUp.clicked) {
		if (type < BLOCK) {
			type++;
		}
	}
	if (controller.buttonDown.clicked) {
		if (type > TRIANGLE_UP) {
			type--;
		}
	}

	if (Input::Key0.clicked) type = TRIANGLE_UP;
	if (Input::Key1.clicked) type = TRIANGLE_DOWN;
	if (Input::Key2.clicked) type = TRIANGLE_RIGHT;
	if (Input::Key3.clicked) type = TRIANGLE_LEFT;
	if (Input::Key4.clicked) type = TRIANGLE_OUT;
	if (Input::Key5.clicked) type = TRIANGLE_DROP;
	if (Input::Key6.clicked) type = BLOCK;
//	if (Input::Key7.clicked) type = TRIANGLE_OUT_1;
//	if (Input::Key8.clicked) type = ;
//	if (Input::Key9.clicked) type = ;

}




