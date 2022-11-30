#pragma once
#include<memory>
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

class Controller;
enum class ControllerType;

class OptionScene :
    public BaseScene
{
public:
    OptionScene(UniqueScene ownScene,ControllerType type, SceneID beforeID);
    ~OptionScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Option; };
private:
    
    UniqueScene beforeScene_;    //前のシーン(主にゲームシーン)
    UniqueScene returningScene_; //シーンを切り替える時用
    SceneID beforeSceneID_;      //前のシーンID

    std::unique_ptr<Controller> controller_;//コントローラ情報

    //ファーストはtmxの文字列セカンドは処理のポインター
    std::map<std::string, void (OptionScene::*)()> selectList_;//処理まとめリスト

    //std::map<std::string, std::string>* listToDraw_;        //描画するリストのポインタ

    //ファーストはtmxの文字列セカンドは画像の文字列
    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
    std::map<std::string, std::string> displayList_;        //描画だけリスト
    std::map<std::string, std::string> controlList_;        //操作リスト
    std::map<std::string, std::string> audioList_;          //音リスト
    std::map<std::string, std::string> graphicList_;        //グラフィックリスト
    std::map<std::string, std::string> titleRetureList_;    //タイトルに行くリスト
    std::map<std::string, std::string> retureList_;         //戻るリスト

    std::map<std::string, std::string> supportList_;         
    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

    TmxObj tmxObj_;     //衝突判定用tmx
    
    std::string selection_; //今選択しているものを格納する場所

    std::pair<std::string, std::string> OldhitMouse_;       //今マウスが当たっているものを格納する場所
    std::pair<std::string,std::string> hitMouse_;       //今マウスが当たっているものを格納する場所

    int Cnt_;       //点滅のカウント
    int Volume_;    //音量
    int brightness_;//明るさ


    //画像にフィルタをかける
    void InitGraphFilter();
    //リストに格納
    void InList();
    //変数の初期化
    void InitVariable();
    //衝突判定
    void HitCollAndAssignProcess();

    /// <summary>
    /// クリックしたとき当たったか
    /// </summary>
    /// <param name="coll">四角形衝突判定</param>
    void HitCollClickAndAssignProcess(std::pair<std::string, Collision> coll);

    /// <summary>
    /// マウスが当たっているか
    /// </summary>
    /// <param name="coll">四角形衝突判定</param>
    std::pair<std::string, std::string> HitCollMouse(std::pair<std::string, Collision> coll);


    //関数ポインタ
    //ーーーーーーーーーーーーーーーーーーーーーー
    void (OptionScene::* UpdateFuncPtr_)();
    
    //更新関数ポインタに代入する関数
    void UpdateControl();         //キー説明
    void UpdateAudio();           //音量
    void UpdateGraphic();         //明るさ調整
    void UpdateToTitle();     // タイトルに行く
    void UpdateReturnScreen();    //戻る

    //描画関数ポインタ
    void (OptionScene::* DrawFuncPtr_)();

    //描画関数ポインタに代入する関数
    void DrawControl();         //キー説明
    void DrawAudio();           //音量
    void DrawGraphic();         //明るさ調整
    void DrawToTitle();     // タイトルに行く
    void DrawReturnScreen();    //戻る
    //ーーーーーーーーーーーーーーーーーーーーー
    
    //まとめたコリジョンのdraw
    void DrawColl();
    /// <summary>
    /// コリジョンのY軸を中心に描画
    /// </summary>
    /// <param name="coll">四角形衝突判定</param>
    /// <param name="imageStr">画像のハンドル用文字列</param>
    void DrawCollCenterY(std::pair<std::string, Collision> coll, std::string imageStr);

    void DrawCollCenter(std::pair<std::string, Collision> coll, std::string imageStr);


    /// <summary>
    /// SelectList専用描画
    /// </summary>
    /// <param name="coll">四角形衝突判定</param>
    void DrawSelectList(std::pair<std::string, Collision> coll);

    /// <summary>
    /// リストの描画
    /// </summary>
    /// <param name="coll">四角形衝突判定</param>
    /// <param name="maplist">描画に使うマップリスト</param>
    void DrawList(std::pair<std::string, Collision> coll, std::map<std::string, std::string> maplist);
};