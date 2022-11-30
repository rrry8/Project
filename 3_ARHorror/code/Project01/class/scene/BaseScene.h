#pragma once
#include<DxLib.h>
#include<memory>
#include<array>

class BaseScene;

using UniqueScene = std::unique_ptr<BaseScene>;

//シーンのID
enum class SceneID
{
	Game,		//ゲーム
	Inventory,	//インベントリ
	Result,		//リザルト
	GameOver,	//ゲームオーバー
	Select,		//セレクト
	Title,		//タイトル
	Option,
	Load,
	Max
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	// 初期化処理
	virtual void Init(void) = 0;

	void InitVertext(void);

	// 更新ステップ
	virtual UniqueScene Update(UniqueScene scene) = 0;

	// 描画処理
	virtual void Draw();
	virtual void DrawPostEffect(int ps);
	virtual void DrawPostEffect(int ps,int texture1,int texture2);
	virtual void DrawScreen(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;

	virtual SceneID GetSceneID(void) = 0;
	
private:

protected:
	//スクリーン情報
	int screenID_;

	//アルベドレンダーターゲット
	int albedRT_;
	//法線レンダーターゲット
	int normalSpecRT_;
	//ワールド座標レンダーターゲット
	int depthRT_;

	//スクリーンの大きさ
	int screenSizeX_;	//横
	int screenSizeY_;	//縦

	//Primitive2D用の頂点データ
	std::array<VERTEX2DSHADER, 4> verts_;
};