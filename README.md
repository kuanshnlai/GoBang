# GoBang
# AI Homework 3


### 0.檔案內容說明:
檔案包含兩個資料夾(console版本/ui版本)
* ui版本資料夾(此版本為最終版本):
   + ai_hw3資料夾(source code和Qt的專案檔)
   + 40670115h_ai_Hw3.exe(打包好的執行檔)
   + readMe.txt(簡易的說明文件)
* console版本(包含source code和code block的專案檔)
### 1.系統規格及開發環境
* IDE:  CodeBlock(console版本)/Qtcreator(ui版本)
* Library: Qt
* 環境: windows 10
* 處理器: Intel® Core™ i5-8250U CPU @ 1.60GHz 1.80GHz
* RAM : 8GB
### 2.程式執行說明
程式執行畫面如圖示:
![](https://i.imgur.com/qDcqLQN.png)
程式區塊說明:
![](https://i.imgur.com/py9x9hI.png)
1. 使用者可以每一步的時間
2. 按下開始遊戲可以開始跟電腦對下
3. 在盤面上呈現棋子樣式說明
4. 倒數計時，時間到還沒有下出棋子則判負
5. 顯示是第幾回合和輪到哪方下棋
6. 遊戲中一些提示訊息
7. 玩家在棋盤點擊需要再按下確認來確認棋子位置(防止誤按)
8. 投降按鈕



### 3.說明設計方法
使用到的資料結構，分為ui設計方面及實際演算方法方面
ui設計方面:(簡短說明)

class MainWindow(整個遊戲視窗的物件)
class Board(呈現在畫面上的棋盤)
class Stone(棋子的種類)
class PaintableObject(可以繪製的元件(用來呈現回合數及說明棋子種類))

實際演算方法使用類別:
class Thread
輪到電腦方下棋時會分為兩個執行緒，主執行緒處理整個視窗的事件，另一個執行緒用來做決策(alpha-beta search)
class simulationBoard
供電腦做決策會使用到的模擬盤面，因為電腦做決策只需要用到整個Board的一部份資料，例如電腦做決策時不需要處理使用者點擊的事件，棋盤的狀態紀錄可以只用一個二維矩陣來記錄，在處理新增棋子時也不用實際的去生成一個stone類別，只要更改二維矩陣上的狀態即可。

`這部分是做得不太好的地方，應該改成simulationBoard為基礎的class，Board class再去繼承他，才不會有太多重複功能的code`

審局函數說明:
審局函數我把它包在simulationBoard的class內
分為兩部分
1. 針對棋子
2. 針對整體盤面
我先用迭代的方式去迭代所有黑棋和白棋，針對每個棋子我有一個打分數的函數get_score()，對於兩個不同棋類各取能夠獲得最大的分數，最後再套用 "白色分數 - 黑色分數*1.2(防守優先)"
擷取程式碼片段如下:
```
int SimulationBoard::get_score(int x,int y)const{
    //判斷棋子分數
    int scoreList[6]={60,70,1250,1300,5000,10000000};//活二 死三 活三 死四 活四 五個棋連線
    int score = 0;
    Type cur = avail_[x][y];
    for(auto d:allDir){
        int tmpX = x;
        int tmpY = y;
        auto diff = this->getDiff(d);
        int dx = diff.first;
        int dy = diff.second;
        int countConnect = 0;
        int jumpCount = 0;
        int tmpScore = 0;
        bool block[] = {false,false};//判斷兩邊有沒有被阻檔
        bool jump = false;//判斷跳棋(中間空一格)
        for(int i=0;i<5;i++){
            if(tmpX<0||tmpY<0||tmpX>=15||tmpY>=15){
                block[0] = true;
                break;
            }
            if(!jump && avail_[tmpX][tmpY] == cur){
                ++countConnect;
            }
            else if(!jump && (avail_[tmpX][tmpY]==Type::empty||avail_[tmpX][tmpY]==Type::pending)){
               jump = true;
            }
            else if(jump && avail_[tmpX][tmpY] == cur){
                ++jumpCount;
            }
            else if(jump && avail_[tmpX][tmpY] != cur){
                break;
            }
            else if(avail_[tmpX][tmpY] != cur && avail_[tmpX][tmpY]!=Type::empty&&/
            avail_[tmpX][tmpY]!=Type::pending){
                block[0] = true;
                break;
            }
            tmpX += dx;
            tmpY += dy;
        }
        tmpX = x - dx;
        tmpY = y - dy;
        if((tmpX>15||tmpY>15||tmpX<0||tmpY<0)||(avail_[tmpX][tmpY] != cur &&/
        avail_[tmpX][tmpY]!=Type::empty && avail_[tmpX][tmpY]!=Type::pending) ){
           //如果反方向位置不合法或者有對方的棋則判斷為阻擋
            block[1] = true;
        }
        int total = jumpCount + countConnect;//判斷盤面有連己的潛力
                //cout<<"Jump  = "<<jumpCount<<"Connect = "<<countConnect<<endl;
        bool live = (!block[0]&&!block[1]); //若兩邊都沒有被擋則為活棋
        bool dead = ((!block[1]&&block[0])||(!block[0]&&block[1]));//一邊被擋則為死棋(死三 死四)
        if(!live && !dead){//非活棋也非死棋則兩邊都被擋則此棋沒有潛力
           tmpScore = 0;
        }
        //live two
        else if((total== 2) && live){
           tmpScore = scoreList[0];
        }
       //dead three
        else if((total == 3) && !live &&dead){
           tmpScore = scoreList[1];
        }
        //live three
        else if((total==3) && live){
           tmpScore = scoreList[2];
        }
        //dead four
        else if((total == 4)&&dead){
           tmpScore = scoreList[3];
        }
                //live four
        else if((total == 4)&&live){
           tmpScore = scoreList[4];
        }
                //five
        else if(total == 5){
           tmpScore = scoreList[5];
        }
        else{
           tmpScore = 0;
        }
        score = std::max(score,tmpScore);
    }
    return score;
}

int SimulationBoard::critic()const{
    //判斷盤面分數
    int whiteScore = 0;
    int blackScore = 0;
    for(auto& it:black_){
        blackScore = std::max(blackScore,get_score(it.first,it.second));//所有黑棋取最大的分數
    }
    for(auto& it:white_){
        whiteScore = std::max(whiteScore,get_score(it.first,it.second));//所有白棋取最大的分數
    }
    return whiteScore-(blackScore*1.2); //因本系統電腦只能是白方(待改進)，回傳分數為白棋分數-黑棋分數*1.2 代表防守較為重要
}
```
測試表現:
因為我不太清楚如何量化程式的棋力，所以以我自身跟他對下的經驗來當作測試表現的總結，
一開始還沒調權重時10局裡面大概還可以贏1局，但是調完權重後幾乎沒有辦法贏它，不過也
可能是因為我不太會下的關係。
優點:
1. ui 介面簡單明瞭
2. 會有錯誤提示(第一手下錯地方)
缺點:
1. 不夠彈性(機器只能是白棋)
2. 沒有外接的方法讓它可以跟其他程式對下
遊戲實際畫面:


//還沒調整活三權重時
![](https://i.imgur.com/QdDoLHJ.png)
//調完活三權重
![](https://i.imgur.com/vQQ8Lg5.png)

### 4.參考資料:
https://www.jb51.net/article/166466.htm (參考審局函數部分)
https://doc.qt.io/qt-5/ (Qt document)
https://openhome.cc/Gossip/Qt4Gossip/index.html (Qt 教學)
### 5.遇到的困難:
一開始使用minmax方法來搜尋，但隨著遊戲的進行，需要搜尋的節點越來越多，
速度有明顯的下降，而且觀察有好多節點(盤面)得到的分數都相同，也就是說真正
關鍵的走步並沒有那麼多，因此後來改用alpha-beta search來做，效果還不錯，
但是若是增加搜索的層數時間又會上升許多，造成遊玩體驗不佳，經過測試將層數定
為三層會有比較好的效果，一開始的時候是實做console版本的，但是實在太難進行
測試，每下一步都需要去對齊，一不小心下錯位置就需要重新再下一次，因此後來花
了一些時間實作ui版本的，這個版本可以讓使用者用滑鼠點擊的方式來下棋，不僅比
較美觀，也比較方便操作，至於缺點的話就是前面所提到的比較不彈性，電腦方永遠
是後手，若要進行跟其他ai對戰的話缺少接口可以進行遊戲，這兩個是比較主要的缺
點，也是之後可以改良的方向。


