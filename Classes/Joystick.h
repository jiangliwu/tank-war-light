#include "cocos2d.h" 
#include "type.h"
using namespace cocos2d; 
  
class Joystick : public CCLayer 
{ 
public: 
    Joystick(void); 
    ~Joystick(void); 
public: 
    CCPoint centerPoint;    // 摇杆中心 
    CCPoint currentPoint;   // 摇杆当前位置 
    bool active;            // 是否激活摇杆 
    float radius;           // 摇杆半径 
    CCSprite *jsSprite;     // 摇杆实例 
	CCSprite *jsBg;
    //************************************ 
    // Method:    Active 
    // FullName:  Joystick::Active 
    // Access:    public  
    // Returns:   void 
    // Qualifier: 设置摇杆功能可用 
    //************************************ 
    void Active(); 
    //************************************ 
    // Method:    Inactive 
    // FullName:  Joystick::Inactive 
    // Access:    public  
    // Returns:   void 
    // Qualifier: 设置摇杆功能不可用 
    //************************************ 
    void Inactive(); 
    //************************************ 
    // Method:    getDirection 
    // FullName:  Joystick::getDirection 
    // Access:    public  
    // Returns:   cocos2d::CCPoint 
    // Qualifier: 获取摇杆方向，这里返回的是单位向量 
    //************************************ 
    CCPoint getDirection(); 


	//////////////////////////////////////////////////////////////////////////
	//getDirectionByDir();
	//
	//////////////////////////////////////////////////////////////////////////

	Direction getDirectionByDir();

    //************************************ 
    // Method:    getVelocity 
    // FullName:  Joystick::getVelocity 
    // Access:    public  
    // Returns:   float 
    // Qualifier: 获取摇杆的力度 
    //************************************ 
    float getVelocity(); 
    //************************************ 
    // Method:    updatePos 
    // FullName:  Joystick::updatePos 
    // Access:    public  
    // Returns:   void 
    // Qualifier: 刷新函数，交给日程管理器 
    // Parameter: ccTime dt 
    //************************************ 
    void updatePos(float dt); 
  
    //************************************ 
    // Method:    JoystickWithCenter 
    // FullName:  Joystick::JoystickWithCenter 
    // Access:    public static  
    // Returns:   Joystick* 
    // Qualifier: 初始化摇杆 
    // Parameter: CCPoint aPoint 摇杆中心 
    // Parameter: float aRadius 摇杆半径 
    // Parameter: CCSprite * aJsSprite 摇杆控制点 
    // Parameter: CCSprite * aJsBg 摇杆背景 
    //************************************ 

	void setOpacity(float op);
    static Joystick* JoystickWithCenter(CCPoint aPoint, float aRadius, CCSprite* aJsSprite, CCSprite* aJsBg); 
    Joystick* initWithCenter(CCPoint aPoint, float aRadius, CCSprite* aJsSprite, CCSprite* aJsBg); 
  
    
	
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent); 
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent); 
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent); 
  
    CREATE_FUNC(Joystick); 
};