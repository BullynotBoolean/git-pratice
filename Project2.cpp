#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

// ============ 공통 유틸 ============
static inline void ClearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============ 상수 ============
const int MIN_HP_MP = 50;
const int MIN_ATK_DEF = 0;
const int RECOVER_HP = 20;
const int RECOVER_MP = 20;
const int SKILL_MP_COST = 50;

// ============ 전방 선언 ============
class World;
class Player;
class Monster;

// ============ setPotion ============
void setPotion(int count, int* p_HPPotion, int* p_MPPotion) {
    *p_HPPotion = count;
    *p_MPPotion = count;
    cout << "* 포션이 지급되었습니다. (HP, MP 포션 각 " << count << "개)\n";
}

// ============ Actor ============
class Actor {
public:
    virtual ~Actor() = default;
    virtual void BeginPlay() {}
    virtual void Tick(float /*dt*/) {}
    void SetWorld(World* w) { WorldPtr = w; }
protected:
    World* WorldPtr = nullptr;
};

// ============ GameState ============
struct GameState {
    int HPpots = 0;
    int MPpots = 0;
    int wave = 1;
};

// ============ World ============
class World {
public:
    template<typename T, typename... Args>
    T* SpawnActor(Args&&... args) {
        T* obj = new T(std::forward<Args>(args)...);
        obj->SetWorld(this);
        Actors.push_back(obj);
        obj->BeginPlay();
        return obj;
    }
    void TickAll(float dt) { for (auto* a : Actors) a->Tick(dt); }
    ~World() { for (auto* a : Actors) delete a; }

    GameState& GetGameState() { return GS; }

private:
    vector<Actor*> Actors;
    GameState GS;
};

// ============ Player ============
class Player : public Actor {
public:
    Player(string nickname)
        : job_name("Beginner"), nickname(nickname), level(1),
        HP(100), MP(100), power(5), defence(5), accuracy(5), speed(5) {
    }

    virtual ~Player() = default;

    virtual void attack() = 0;
    virtual void attack(Monster* monster) = 0;

    void printPlayerStatus() {
        cout << "------------------------------------\n";
        cout << "* 현재 능력치\n";
        cout << "닉네임: " << nickname << "\n";
        cout << "직업: " << job_name << "\n";
        cout << "Lv. " << level << "\n";
        cout << "HP: " << HP << "\n";
        cout << "MP: " << MP << "\n";
        cout << "공격력: " << power << "\n";
        cout << "방어력: " << defence << "\n";
        cout << "정확도: " << accuracy << "\n";
        cout << "속도: " << speed << "\n";
        cout << "------------------------------------\n";
    }

    // getter
    string getJobName() const { return job_name; }
    string getNickname() const { return nickname; }
    int getLevel()     const { return level; }
    int getHP()        const { return HP; }
    int getMP()        const { return MP; }
    int getPower()     const { return power; }
    int getDefence()   const { return defence; }
    int getAccuracy()  const { return accuracy; }
    int getSpeed()     const { return speed; }

    // setter
    void setNickname(string s) { nickname = std::move(s); }
    bool setHP(int v) { if (v >= 1) { HP = v; return true; } HP = 0; return false; }
    bool setMP(int v) { if (v >= 1) { MP = v; return true; } MP = 0; return false; }
    void setPower(int v) { power = v; }
    void setDefence(int v) { defence = v; }
    void setAccuracy(int v) { accuracy = v; }
    void setSpeed(int v) { speed = v; }

protected:
    string job_name;
    string nickname;
    int level;
    int HP, MP;
    int power, defence, accuracy, speed;
};

// ============ Monster ============
class Monster : public Actor {
public:
    explicit Monster(string name) : name(std::move(name)), HP(10), power(30), defence(10), speed(10) {}
    void BeginPlay() override { cout << "[" << name << "] 이(가) 나타났다!\n"; }

    void attack(Player* player);

    string getName()   const { return name; }
    int    getHP()     const { return HP; }
    int    getPower()  const { return power; }
    int    getDefence()const { return defence; }
    int    getSpeed()  const { return speed; }

    void setName(string n) { name = std::move(n); }
    bool setHP(int v) { if (v >= 1) { HP = v; return true; } HP = 0; return false; }
    void setPower(int v) { power = v; }
    void setDefence(int v) { defence = v; }
    void setSpeed(int v) { speed = v; }

private:
    string name;
    int HP;
    int power;
    int defence;
    int speed;
};

// ============ 직업 클래스 ============
class Warrior : public Player {
public:
    explicit Warrior(string nickname) : Player(std::move(nickname)) { job_name = "Warrior"; }
    void attack() override { cout << "[전사]가 공격했다!\n"; }
    void attack(Monster* monster) override;
};
class Magician : public Player {
public:
    explicit Magician(string nickname) : Player(std::move(nickname)) { job_name = "Magician"; }
    void attack() override { cout << "[마법사]가 공격했다!\n"; }
    void attack(Monster* monster) override;
};
class Archer : public Player {
public:
    explicit Archer(string nickname) : Player(std::move(nickname)) { job_name = "Archer"; }
    void attack() override { cout << "[궁수]가 공격했다!\n"; }
    void attack(Monster* monster) override;
};
class Thief : public Player {
public:
    explicit Thief(string nickname) : Player(std::move(nickname)) { job_name = "Thief"; }
    void attack() override { cout << "[도적]가 공격했다!\n"; }
    void attack(Monster* monster) override;
};

// ============ Monster::attack 구현 ============
void Monster::attack(Player* player) {
    if (!player) return;
    int damage = max(1, power - player->getDefence());
    cout << "* " << name << "의 공격! " << damage << " 피해!\n";
    bool alive = player->setHP(player->getHP() - damage);
    if (alive) cout << "플레이어 HP: " << player->getHP() << "\n";
    else       cout << "플레이어가 쓰러졌습니다.\n";
}

// ============ 각 직업의 공격 ============
void Warrior::attack(Monster* monster) {
    if (!monster) { cout << "공격 대상이 없습니다.\n"; return; }
    int damage = max(1, getPower() - monster->getDefence());
    cout << monster->getName() << "에게 " << damage << "의 피해!\n";
    bool alive = monster->setHP(monster->getHP() - damage);
    if (alive) cout << "몬스터 HP: " << monster->getHP() << "\n";
    else       cout << "몬스터가 쓰러졌습니다! 승리!\n";
}
void Magician::attack(Monster* monster) {
    if (!monster) { cout << "공격 대상이 없습니다.\n"; return; }
    int damage = max(1, getPower() - monster->getDefence());
    cout << monster->getName() << "에게 마법으로 " << damage << "의 피해!\n";
    bool alive = monster->setHP(monster->getHP() - damage);
    if (alive) cout << "몬스터 HP: " << monster->getHP() << "\n";
    else       cout << "몬스터가 쓰러졌습니다! 승리!\n";
}
void Archer::attack(Monster* monster) {
    if (!monster) { cout << "공격 대상이 없습니다.\n"; return; }
    int base = max(1, getPower() - monster->getDefence());
    int per = max(1, base / 3);
    cout << "* 궁수 3연사 (타당 " << per << ")\n";
    for (int i = 1; i <= 3 && monster->getHP() > 0; ++i) {
        cout << "  - [" << i << "] " << monster->getName() << "에게 " << per << "의 피해!\n";
        bool alive = monster->setHP(monster->getHP() - per);
        if (!alive) { cout << "몬스터가 쓰러졌습니다! 승리!\n"; break; }
    }
    if (monster->getHP() > 0) cout << "몬스터 HP: " << monster->getHP() << "\n";
}
void Thief::attack(Monster* monster) {
    if (!monster) { cout << "공격 대상이 없습니다.\n"; return; }
    int base = max(1, getPower() - monster->getDefence());
    int per = max(1, base / 5);
    cout << "* 도적 5연타 (타당 " << per << ")\n";
    for (int i = 1; i <= 5 && monster->getHP() > 0; ++i) {
        cout << "  - [" << i << "] " << monster->getName() << "에게 " << per << "의 피해!\n";
        bool alive = monster->setHP(monster->getHP() - per);
        if (!alive) { cout << "몬스터가 쓰러졌습니다! 승리!\n"; break; }
    }
    if (monster->getHP() > 0) cout << "몬스터 HP: " << monster->getHP() << "\n";
}

// ============ Controller ============
enum class Command { None = 0, RecoverHP = 1, RecoverMP = 2, BoostHP = 3, BoostMP = 4, Skill = 5, Ult = 6, Attack = 7, Quit = 8 };

class PlayerController : public Actor {
public:
    Command GetCommand() {
        cout << "번호를 선택해주세요 : ";
        int n;
        if (!(cin >> n)) { ClearInput(); cout << "정수를 입력하세요.\n"; return Command::None; }
        if (n < 1 || n > 8) { cout << "1~8 중에서 선택하세요.\n"; return Command::None; }
        return static_cast<Command>(n);
    }
};

// ============ GameMode ============
class GameMode : public Actor {
public:
    void PrintMenu() const {
        cout << "=============================================\n";
        cout << "< 스탯/전투 시스템(엔진풍) >\n"
            << "1. HP 회복(+20, HP포션-1)\n"
            << "2. MP 회복(+20, MP포션-1)\n"
            << "3. HP 강화(x2)\n"
            << "4. MP 강화(x2)\n"
            << "5. 공격 스킬(MP -50, 공격 수행)\n"
            << "6. 필살기(MP 50% 소모, 공격 수행)\n"
            << "7. 공격(일반)\n"
            << "8. 나가기\n";
    }

    void InitStatsInteractive(Player& P, GameState& GS) const {
        int hp, mp, atk, def;
        while (true) {
            cout << "초기 HP와 MP를 입력하세요(HP/MP는 50 초과): ";
            if (!(cin >> hp >> mp)) { ClearInput(); cout << "정수를 입력하세요.\n"; continue; }
            if (hp <= MIN_HP_MP || mp <= MIN_HP_MP) { cout << "HP/MP가 너무 작습니다. 다시.\n"; continue; }
            break;
        }
        while (true) {
            cout << "초기 ATK와 DEF를 입력하세요(둘 다 0 초과): ";
            if (!(cin >> atk >> def)) { ClearInput(); cout << "정수를 입력하세요.\n"; continue; }
            if (atk <= MIN_ATK_DEF || def <= MIN_ATK_DEF) { cout << "ATK/DEF는 0보다 커야 합니다. 다시.\n"; continue; }
            break;
        }
        P.setHP(hp); P.setMP(mp); P.setPower(atk); P.setDefence(def);

        setPotion(5, &GS.HPpots, &GS.MPpots);
        P.printPlayerStatus();
    }

    void Render(const Player& P, const Monster& M, const GameState& GS) const {
        cout << "\n[플레이어] " << P.getNickname() << " (" << P.getJobName() << ") "
            << "HP:" << P.getHP() << " MP:" << P.getMP()
            << " ATK:" << P.getPower() << " DEF:" << P.getDefence()
            << " | 포션(HP:" << GS.HPpots << ", MP:" << GS.MPpots << ")\n";
        cout << "[몬스터] " << M.getName()
            << "  HP:" << M.getHP()
            << " DEF:" << M.getDefence()
            << " SPD:" << M.getSpeed()
            << " (Wave " << GS.wave << ")\n";
    }

    void Apply(Player& P, Monster*& M, GameState& GS, Command cmd, bool& running, World& world) {
        bool playerDidAttack = false;
        bool skipCounterThisTurn = false;

        switch (cmd) {
        case Command::RecoverHP:
            if (GS.HPpots <= 0) { cout << "HP 포션이 부족합니다.\n"; break; }
            P.setHP(P.getHP() + RECOVER_HP); --GS.HPpots;
            cout << "HP +" << RECOVER_HP << " (HP포션 -1)\n";
            break;

        case Command::RecoverMP:
            if (GS.MPpots <= 0) { cout << "MP 포션이 부족합니다.\n"; break; }
            P.setMP(P.getMP() + RECOVER_MP); --GS.MPpots;
            cout << "MP +" << RECOVER_MP << " (MP포션 -1)\n";
            break;

        case Command::BoostHP:
            P.setHP(max(1, P.getHP() * 2));
            cout << "HP가 2배로 증가되었습니다. 현재 HP: " << P.getHP() << "\n";
            break;

        case Command::BoostMP:
            P.setMP(max(1, P.getMP() * 2));
            cout << "MP가 2배로 증가되었습니다. 현재 MP: " << P.getMP() << "\n";
            break;

        case Command::Skill:
            if (P.getMP() < SKILL_MP_COST) { cout << "스킬 사용 불가(MP 부족).\n"; break; }
            P.setMP(P.getMP() - SKILL_MP_COST);
            cout << "* 스킬 사용: MP -" << SKILL_MP_COST << "\n";
            P.attack(M);
            playerDidAttack = true;
            break;

        case Command::Ult:
            if (P.getMP() <= 0) { cout << "필살기 사용 불가(MP 0).\n"; break; }
            P.setMP(P.getMP() / 2);
            cout << "* 필살기: MP 50% 소모. 현재 MP: " << P.getMP() << "\n";
            P.attack(M);
            playerDidAttack = true;
            break;

        case Command::Attack:
            P.attack(M);
            playerDidAttack = true;
            break;

        case Command::Quit:
            cout << "프로그램을 종료 합니다.\n";
            running = false;
            return;

        default: break;
        }


        if (M->getHP() <= 0) {
            ++GS.wave;
            M = world.SpawnActor<Monster>("슬라임");
            cout << "\n=== 새 몬스터가 나타났다! (Wave " << GS.wave << ") ===\n";
            skipCounterThisTurn = true;
        }

        if (!skipCounterThisTurn && M->getHP() > 0) {
            M->attack(&P);
        }

        if (P.getHP() <= 0) {
            cout << "게임 오버.\n";
            running = false;
        }
    }
};

// ============ 메뉴 출력 ============
void printMenu() {
    cout << "=============================================\n";
    cout << "< 스탯/전투 시스템(엔진풍) >\n"
        << "1. HP 회복 (+20, HP포션 -1)\n"
        << "2. MP 회복 (+20, MP포션 -1)\n"
        << "3. HP 강화 (x2)\n"
        << "4. MP 강화 (x2)\n"
        << "5. 공격 스킬 (MP -50행)\n"
        << "6. 필살기 (MP 50% 소모)\n"
        << "7. 공격 (몬스터 타격)\n"
        << "8. 나가기\n";
}

// ============ main ============
int main() {
    World world;
    GameMode* gm = world.SpawnActor<GameMode>();
    PlayerController* pc = world.SpawnActor<PlayerController>();

    // 플레이어 생성
    cout << "* 닉네임을 입력: ";
    string nickname; cin >> nickname;

    cout << "* 직업 선택: 1.전사 2.마법사 3.궁수 4.도적\n선택: ";
    int job_choice = 0; cin >> job_choice;

    Player* player = nullptr;
    switch (job_choice) {
    case 1: player = world.SpawnActor<Warrior>(nickname);  break;
    case 2: player = world.SpawnActor<Magician>(nickname); break;
    case 3: player = world.SpawnActor<Archer>(nickname);   break;
    case 4: player = world.SpawnActor<Thief>(nickname);    break;
    default: player = world.SpawnActor<Warrior>(nickname); break;
    }

    // 초기 스탯/포션 세팅
    gm->InitStatsInteractive(*player, world.GetGameState());

    // 몬스터 생성
    Monster* monster = world.SpawnActor<Monster>("슬라임");

    // 루프
    bool running = true;
    while (running) {
        gm->Render(*player, *monster, world.GetGameState());
        printMenu();

        Command cmd = pc->GetCommand();
        gm->Apply(*player, monster, world.GetGameState(), cmd, running, world);

        world.TickAll(0.0f);
    }
    return 0;
}
