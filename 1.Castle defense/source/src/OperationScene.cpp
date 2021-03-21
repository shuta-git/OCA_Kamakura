//---------------------------------------------
// 定数の宣言
//---------------------------------------------
const int CENTER_POS_X = WINDOW_W / 2;
const int CENTER_POS_Y = WINDOW_H / 2;

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
OperationScene::OperationScene(int old_scene_)
{
	scene_type = OPERATION;
	old_scene = old_scene_;

	SetGraphHandle("operation/operation_background.png", CENTER_POS_X, CENTER_POS_Y);
	SetGraphHandle("operation/operation_background1.png", CENTER_POS_X, CENTER_POS_Y,0.6f);
	SetGraphHandle("operation/operation_frame.png", CENTER_POS_X, CENTER_POS_Y,0.5f);
	SetGraphHandle("operation/operation_letter.png", CENTER_POS_X, 130,0.5f);
	
	SetGraphHandle("operation/controller.png", CENTER_POS_X, CENTER_POS_Y,1.1f);

	font_handle.push_back(CreateFontToHandle(NULL, 60, -1));
}
//---------------------------------------------
//  デストラクタ
//---------------------------------------------
OperationScene::~OperationScene()
{
	
}
//---------------------------------------------
// 更新処理
//---------------------------------------------
void OperationScene::Update()
{
	if (IsKeyOn(KEY_INPUT_C) || IsPadOn(PAD_A)) {
		scene_type = old_scene;
	}
}
//---------------------------------------------
// 描画処理
//---------------------------------------------
void OperationScene::Draw(){
	// 画像描画
	DrawGraphHandle();
	// 戻る文字の描画
	DrawCenterString(CENTER_POS_X, CENTER_POS_Y + 350, "A : 戻る", BLACK, font_handle[0]);
}
