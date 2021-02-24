#include "GameMain.h"

//----------------------------------------------------------------------
// �R���X�g���N�^
//----------------------------------------------------------------------
GameMain::GameMain(int num)
{
	map_num = num;
	goal = false;
	Init();

	//----------------------------------------------------------------------
	// �R���g���[���[��DeadZone���Ă�
	//----------------------------------------------------------------------
	controller.setLeftThumbDeadZone();
	controller.setRightThumbDeadZone();
	controller.setLeftTriggerDeadZone();
	controller.setRightTriggerDeadZone();

	//----------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------
	// MAP
	map = Create<Map>();
	// �u���b�N
	TextReadCreatBlock();
	// �g���b�v
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
// �f�X�g���N�^
//----------------------------------------------------------------------
GameMain::~GameMain()
{
	//�S�^�X�N�̏���
	Task::All::Clear();
}
//----------------------------------------------------------------------
// ����������
//----------------------------------------------------------------------
void GameMain::Init()
{
	tout_cnt = 0;
	tout_cnt1 = 0;
	tout_add = 0;
	tout_add1 = 0;
}
//----------------------------------------------------------------------
// �X�V����
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
	// �g���b�v�̎�ނ�ς���
	KeyChangeTrapType();

	// R�L�[���������Ƃ����X�|�[��
	if (Input::KeyR.clicked || controller.buttonY.clicked && map->GetPDeath()) {
		Init();
		// �u���b�N�𐶐�
		TextReadCreatBlock();
		// �����e�L�X�g��ǂݍ���
		InitTextCreatTrap();
		WriterTextTrap();
		WriterTextDropTrap();
	}
	// ���N���b�N�����Ƃ��Ƀt�@�C���̒��g����������
	if (Input::MouseL.clicked ) {
		// 0�@���� 6 �܂ł̊�
		if (type >= TRIANGLE_UP && type < BLOCK ) {
			// �^�C�v�� 6 �i�h���b�v�g���b�v�j�łȂ��Ƃ�
			if (type != TRIANGLE_DROP) {
				WriterText2Trap();
				WriterTextTrap();
			}// 6 �̎�
			else {
				WriterText2DropTrap();
				WriterTextDropTrap();
			}
			InitWriterTextTrap();
		}
		// type���X�̂Ƃ��u���b�N�쐬
		if (type == BLOCK) {
			ChangeTextBlock();
		}
		// �g���b�v
		TextReadCreatBlock();
		// �g���b�v�𐶐�
		TextReadCreatTrap();
		TextReadCreatDropTrap();
	}
	// �E�N���b�N
	if (Input::MouseR.clicked) {
		DeleteTrap();
		WriterTextTrap();
		WriterTextDropTrap();
		InitWriterTextTrap();
		// �g���b�v
		TextReadCreatBlock();
		// �g���b�v�𐶐�
		TextReadCreatTrap();
		TextReadCreatDropTrap();
	}
#else
#endif

	//TaskSend �� TaskReceive �Őݒ肵���֐����Ăяo��
	TaskSend::All::Update();
	//TaskCall �Őݒ肵���֐����Ăяo��
	TaskCall::All::Update(CallGroup_Update);

	// TriangleOut�͈̔͂ɓ�������
	if (map->GetTOutRange() == true) {
		ChangeTrap();
		WriterTextTrap();
		// �g���b�v�𐶐�
		TextReadCreatOutTrap1();
		map->SetTOutRange();
	}
	// �S�[���̐Ԃ��l�p�Ƀv���C���[���G���ƃN���A
	if (map->GetPlayer().intersects(GetRectGoal())) {
		goal = true;
	}

}
//----------------------------------------------------------------------
// �`�揈��
//----------------------------------------------------------------------
void GameMain::Draw()
{

	const auto t1 = camera.createTransformer();
	TaskCall::All::Update(CallGroup_Draw);

#if ! defined(NDEBUG)
	// b_pos�Ƀ}�E�X�|�W�V�������u���b�N�̃T�C�Y�Ŋ������l������
	b_pos = Mouse::Pos() / MAP_SIZE;
	t_pos = Mouse::Pos();
	//�@�J�[�\��
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

	// �S�[���̕`��
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
// �I������
//----------------------------------------------------------------------
void GameMain::Exit()
{
}
//----------------------------------------------------------------------
// �擾�������ԍ��ƍ��W�̎O�p�`���擾
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
// MAP�e�L�X�g�t�@�C����ǂݍ���Ńu���b�N�����
//----------------------------------------------------------------------
void GameMain::TextReadCreatBlock()
{
	// ����800 ��@-320
	// ���@�O�@�E�@1200
	// �ŏ��Ƀu���b�N�����ׂď���
	Block::Task::All::Clear();
	// MAP.txt�t�@�C����ǂ�
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
			// �u���b�N
			Create<Block>(map->GetWorld(), b_pos.x, b_pos.y);
		}
		b_pos.x += 1;
	}
	reader.close();
}
//----------------------------------------------------------------------
// MAP�e�L�X�g��ǂݍ���
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
// MAP�e�L�X�g�ɏ�������
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
// MAP�e�L�X�g�̒��g�̏�������
//----------------------------------------------------------------------
void GameMain::ChangeTextBlock()
{	
	// �P��������
	if (MAP_text[(int)b_pos.y * BLOCK_W + (int)b_pos.x] == '1') {
		MAP_text[(int)b_pos.y * BLOCK_W + (int)b_pos.x] = '0';
	}
	else {// �O��������
		MAP_text[(int)b_pos.y * BLOCK_W + (int)b_pos.x] = '1';
	}
	// MAP�e�L�X�g�ɏ�������
	WriterTextMAP();
	
}
//----------------------------------------------------------------------
// �ŏ������Ă΂��֐�( Init �� Trap2 , DropTrap2 )
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
// �����̃e�L�X�g��Trap�e�L�X�g���㏑���ۑ�( Trap , DropTrap �� Init ) 
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
// �g���b�v�e�L�X�g��ǂݍ���( Trap )
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
// �g���b�v�e�L�X�g��ǂݍ��ݔ�яo��g���b�v���쐬( Trap )
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

		// �t�@�C���̒��̃g���b�v�𐔂���
		tout_cnt1++;

		// num �� 7(TRIANGLE_OUT_1)�̎�
		if (num == TRIANGLE_OUT_1) {
			// �t�@�C���ɏ����ꂽ�ꏊ�������ŁA����ȏ�t�@�C���� 4(TRIANGLE_OUT)���Ȃ���
			if (tout_cnt == tout_cnt1 && tout_add != tout_add1) {
				// �N���G�C�g����
				Create<Trap>(map->GetWorld(), pos.x, pos.y, num);
				// �N���G�C�g���ꂽ���𐔂���
				tout_add1++;
			}
		}
	}
	reader.close();
}
//----------------------------------------------------------------------
// �g���b�v�e�L�X�g�ɏ�������( Trap2 �� Trap )
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
// �g���b�v2�e�L�X�g�ɏ�������( Trap �� Trap2 )
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
// �g���b�v������( Trap �� Trap2 )( DropTrap �� DropTrap2)
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
// �g���b�v���`�F���W����( Trap �� Trap2 )
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

		// �t�@�C���̒��̃g���b�v�𐔂���
		count++;

		// �v���C���[��TrapOut�͈̔͂̓����蔻��
		if (Circle(pos.x + MAP_SIZE / 2, pos.y + MAP_SIZE / 2, MAP_SIZE + 20).intersects(map->GetPlayer())) {
			// num ���@�S�iTRIANGLE_OUT�j�̎�
			if (num == TRIANGLE_OUT) {
				// num �̏�������
				num = TRIANGLE_OUT_1;
				// tout_cnt �ɉ��ڂ����
				tout_cnt = count;
				// ����������ꂽ���𐔂���
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
// �h���b�v�g���b�v�e�L�X�g��ǂݍ���( DropTrap )
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
// �g���b�v�e�L�X�g�ɏ�������( DropTrap2 �� DropTrap )
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
// �g���b�v�e�L�X�g�ɏ�������( DropTrap �� DropTrap2 )
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
// �g���b�v�̎�ނ��L�[�Ő؂�ւ�
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




