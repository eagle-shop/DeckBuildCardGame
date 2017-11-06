#ifndef General_hpp
#define General_hpp

#include <vector>
#include <string>
#include <map>

using namespace std;

// 文字列判定用マクロ
#define STR_SEARCH(str, word) ((str->find(word) != string::npos) ? true : false)

// 文字列に指定した文字が存在していれば、それを返す
void getString(const string **dst, const string *allString, vector<const string*> *partString);

// とりあえずプリミティブ型を再定義してみる
using CARDNUM           = unsigned long;    // カード枚数
using CARDINDEX         = unsigned long;    // カード番号
using ACTIONNUM         = unsigned long;    // アクション数
using BUYNUM            = unsigned long;    // 購入数
using COINNUM           = unsigned long;    // コイン数
using ACTIONRELATIVE    = long;             // アクション相対数
using BUYRELATIVE       = long;             // 購入数
using COINRELATIVE      = long;             // コイン相対数
using DRAWCARDNUM       = unsigned long;    // 引くカード枚数
using TIME              = double;           // システム時刻


//////////////////////////////
// 列挙型定義群
//////////////////////////////
// フェーズ定義
enum Phase{
    PHASE_UNKNOWN,          // 指定なし
    PHASE_ACTION,           // アクションフェーズ
    PHASE_ACTIONWAIT,       // アクションフェーズ（アクション時にユーザ操作する場合のフェーズ）
    PHASE_BUY,              // 購入フェーズ
    PHASE_CLEANUP,          // クリーンアップフェーズ
    PHASE_OTHERPLAYER,      // 自分以外のプレイヤーのフェーズ
};

// カード種別
enum CardKind{
    KIND_TREASURE,          // 財宝
    KIND_CURSE,             // 呪い
    KIND_VICTORY,           // 勝利点
    KIND_ACTION,            // アクション
    KIND_OTHER,             // その他
};

// カード表示状態
enum DrawState{
    DRAW_NOMAL,             // 通常
    DRAW_SELECT,            // 選択
    DRAW_TONEDOWN,          // トーンダウン
};

enum ActionDone{
    ACTIONDONE,
    ACTIONUNDONE,
    ACTIONCONTINUE,
    ACTIONWAIT,
};

enum Direction{
    FRONT,                  // 正順
    BACK,                   // 逆順
};

enum OPE_ACTION{
    NOACTION = 0,           // アクションしない
    SELECTHANDCARD,         // 手札のカードを選択する
    GOTOTRASH,              // 選択したカードを廃棄置き場へ
    GOTODISCARD,            // 選択したカードを捨札へ
    GOTOHANDCARD,           // 選択したカードを手札へ
    DECKOPEN,               // 山札を公開
};

// タップかドラッグか
enum TAPORDRAG{
    TAP,
    DRAG,
    NOTHING,
};

enum EVENT_KIND{
    EVENT_SELF,
    EVENT_P2P_CONNECTED,
    EVENT_P2P_RECEIVEDATA,
};

constexpr int STRING_MAX = 64;  // 文字列の最大サイズ

//////////////////////////////
// constな文字列群
//////////////////////////////
const string STRNOTHING         = "";

const string PLAYAREA           = "playArea";
const string SUPPLYAREA         = "supplyArea";
const string TRASHAREA          = "trashArea";

// （追加したらALLELEにも追加すること）
const string PHASESWENDACTION   = "swEndAction";
const string PHASESWENDBUY      = "swEndBuy";
const string PHASESWENDTURN     = "swEndTurn";
const string OTHERSWCOMPLETE    = "swComplete";
const string OTHERSWCHANSEL     = "swChansel";

// エリア名（追加したらALLAREAにも追加すること）
using AREA = const string;

AREA SUPPLY     = "supply";
AREA TRASH      = "trash";
AREA HAND       = "hand";
AREA DECK       = "deck";
AREA DISCARD    = "discard";
AREA PLAY       = "play";
AREA PHASE      = "phase";
AREA OTHER      = "other";

// 全エリア定義
#define ALLAREA {\
    &SUPPLY,\
    &TRASH,\
    &HAND,\
    &DECK,\
    &DISCARD,\
    &PLAY,\
    &PHASE,\
    &OTHER,\
}

using COST = unsigned long;

// カード情報構造体
typedef struct{
    CardKind    kind;
    COST        cost;
    COINNUM     coin;
}EFFECTINFO;

typedef struct{
    EFFECTINFO  effectInfo;
    string      fileName;
}CARDINFO;

//////////////////////////////
// カードリスト（追加したらALLELEとCARDLISTにも追加すること）
// サプライとプレイヤーカードクラスにも
//////////////////////////////
const string COPPER     = "COPPER";     // 銅貨
const string SILVER     = "SILVER";     // 銀貨
const string GOLD       = "GOLD";       // 金貨
const string ESTATE     = "ESTATE";     // 屋敷
const string DUCHY      = "DUCHY";      // 公領
const string PROVINCE   = "PROVINCE";   // 属州
const string CURSE      = "CURSE";      // 呪い
const string CHAPEL     = "CHAPEL";     // 礼拝堂
const string CELLAR     = "CELLAR";     // 地下貯蔵庫
const string WOODCUTTER = "WOODCUTTER"; // 木こり
const string SMITHY     = "SMITHY";     // 鍛冶屋
const string MARKET     = "MARKET"; // 市場
const string LABOTATORY = "LABOTATORY"; // 研究所
const string FESTIVAL   = "FESTIVAL";   // 祝祭
const string ADVENTURER   = "ADVENTURER";   // 冒険者
const string TRASHCARD  = "TRASH";      // 廃棄置き場
const string BACKCARD   = "BACKCARD";   // カードの裏面

#define ALLELE {\
    &PHASESWENDACTION,\
    &PHASESWENDBUY,\
    &PHASESWENDTURN,\
    &OTHERSWCOMPLETE,\
    &OTHERSWCHANSEL,\
    &COPPER,\
    &SILVER,\
    &GOLD,\
    &ESTATE,\
    &DUCHY,\
    &PROVINCE,\
    &CURSE,\
    &CHAPEL,\
    &CELLAR,\
    &WOODCUTTER,\
    &SMITHY,\
    &MARKET,\
    &LABOTATORY,\
    &FESTIVAL,\
    &ADVENTURER,\
    &TRASH,\
    &BACKCARD,\
}

#define CARDLIST {\
    /* 名称，        種類，            コスト，コイン，  ファイル名，　*/\
    {COPPER,        {{KIND_TREASURE, 0,     1},     {"006.png"}}},         /* 銅貨 */\
    {SILVER,        {{KIND_TREASURE, 3,     2},     {"007.png"}}},         /* 銀貨 */\
    {GOLD,          {{KIND_TREASURE, 6,     3},     {"008.png"}}},         /* 金貨 */\
    {ESTATE,        {{KIND_VICTORY,  2,     0},     {"002.png"}}},         /* 屋敷 */\
    {DUCHY,         {{KIND_VICTORY,  5,     0},     {"003.png"}}},         /* 公領 */\
    {PROVINCE,      {{KIND_VICTORY,  8,     0},     {"004.png"}}},         /* 属州 */\
    {CURSE,         {{KIND_CURSE,    0,     0},     {"005.png"}}},         /* 呪い */\
    {CHAPEL,        {{KIND_ACTION,   2,     0},     {"010.png"}}},         /* 礼拝堂 */\
    {CELLAR,        {{KIND_ACTION,   2,     0},     {"011.png"}}},         /* 地下貯蔵庫 */\
    {WOODCUTTER,    {{KIND_ACTION,   3,     0},     {"013.png"}}},         /* 木こり */\
    {SMITHY,        {{KIND_ACTION,   4,     0},     {"025.png"}}},         /* 鍛冶屋 */\
    {MARKET,        {{KIND_ACTION,   5,     0},     {"028.png"}}},         /* 市場 */\
    {LABOTATORY,    {{KIND_ACTION,   5,     0},     {"029.png"}}},         /* 研究所 */\
    {FESTIVAL,      {{KIND_ACTION,   5,     0},     {"030.png"}}},         /* 祝祭 */\
    {ADVENTURER,    {{KIND_ACTION,   6,     0},     {"033.png"}}},         /* 冒険者 */\
    {TRASHCARD,     {{KIND_OTHER,    0,     0},     {"001.png"}}},         /* 廃棄置き場 */\
    {BACKCARD,      {{KIND_OTHER,    0,     0},     {"035.png"}}},         /* カードの裏面 */\
}

// カード情報シングルトンクラス（参照のみ可）
class CardInfo{
public:
    CardInfo(const CardInfo&) = delete;
    CardInfo& operator=(const CardInfo&) = delete;
    CardInfo(CardInfo&&) = delete;
    CardInfo& operator=(CardInfo&&) = delete;
    static CardInfo& getInstance(){
        static CardInfo inst;
        return inst;
    }
    // カード情報
    map<const string, CARDINFO> _cardInfo;

private:
    CardInfo():_cardInfo(CARDLIST){};
    ~CardInfo() = default;
};

#endif /* General_hpp */
