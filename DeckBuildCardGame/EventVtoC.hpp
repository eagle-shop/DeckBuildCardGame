#ifndef EventVtoC_hpp
#define EventVtoC_hpp

#include <string>

enum TOUCHMOVE{
    TOUCHMOVE_ENABLE,
    TOUCHMOVE_DISABLE,
};

//typedef void (^BLK)(void);

typedef struct{
    const std::string   *touchFrameName;    // タッチフレーム
    TOUCHMOVE           moveFlg;            // ドラッグ対象かどうか
    const std::string   *parentName;        // ドラッグ対象なら、その親ノード
    void                (^blk)(void);       // ブロック処理（ドラッグなら完了後、タップなら押下時）
//    BLK blk;
}EVENT_VtoC;

#endif /* EventVtoC_hpp */
