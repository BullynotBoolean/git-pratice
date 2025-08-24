#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

// ============ ���� ��ƿ ============
static inline void ClearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============ ��� ============
const int MIN_HP_MP = 50;
const int MIN_ATK_DEF = 0;
const int RECOVER_HP = 20;
const int RECOVER_MP = 20;
const int SKILL_MP_COST = 50;

// ============ ���� ���� ============
class World;
class Player;
class Monster;

// ============ setPotion ============
void setPotion(int count, int* p_HPPotion, int* p_MPPotion) {
    *p_HPPotion = count;
    *p_MPPotion = count;
    cout << "* ������ ���޵Ǿ����ϴ�. (HP, MP ���� �� " << count << "��)\n";
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
        cout << "* ���� �ɷ�ġ\n";
        cout << "�г���: " << nickname << "\n";
        cout << "����: " << job_name << "\n";
        cout << "Lv. " << level << "\n";
        cout << "HP: " << HP << "\n";
        cout << "MP: " << MP << "\n";
        cout << "���ݷ�: " << power << "\n";
        cout << "����: " << defence << "\n";
        cout << "��Ȯ��: " << accuracy << "\n";
        cout << "�ӵ�: " << speed << "\n";
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
    void BeginPlay() override { cout << "[" << name << "] ��(��) ��Ÿ����!\n"; }

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

// ============ ���� Ŭ���� ============
class Warrior : public Player {
public:
    explicit Warrior(string nickname) : Player(std::move(nickname)) { job_name = "Warrior"; }
    void attack() override { cout << "[����]�� �����ߴ�!\n"; }
    void attack(Monster* monster) override;
};
class Magician : public Player {
public:
    explicit Magician(string nickname) : Player(std::move(nickname)) { job_name = "Magician"; }
    void attack() override { cout << "[������]�� �����ߴ�!\n"; }
    void attack(Monster* monster) override;
};
class Archer : public Player {
public:
    explicit Archer(string nickname) : Player(std::move(nickname)) { job_name = "Archer"; }
    void attack() override { cout << "[�ü�]�� �����ߴ�!\n"; }
    void attack(Monster* monster) override;
};
class Thief : public Player {
public:
    explicit Thief(string nickname) : Player(std::move(nickname)) { job_name = "Thief"; }
    void attack() override { cout << "[����]�� �����ߴ�!\n"; }
    void attack(Monster* monster) override;
};

// ============ Monster::attack ���� ============
void Monster::attack(Player* player) {
    if (!player) return;
    int damage = max(1, power - player->getDefence());
    cout << "* " << name << "�� ����! " << damage << " ����!\n";
    bool alive = player->setHP(player->getHP() - damage);
    if (alive) cout << "�÷��̾� HP: " << player->getHP() << "\n";
    else       cout << "�÷��̾ ���������ϴ�.\n";
}

// ============ �� ������ ���� ============
void Warrior::attack(Monster* monster) {
    if (!monster) { cout << "���� ����� �����ϴ�.\n"; return; }
    int damage = max(1, getPower() - monster->getDefence());
    cout << monster->getName() << "���� " << damage << "�� ����!\n";
    bool alive = monster->setHP(monster->getHP() - damage);
    if (alive) cout << "���� HP: " << monster->getHP() << "\n";
    else       cout << "���Ͱ� ���������ϴ�! �¸�!\n";
}
void Magician::attack(Monster* monster) {
    if (!monster) { cout << "���� ����� �����ϴ�.\n"; return; }
    int damage = max(1, getPower() - monster->getDefence());
    cout << monster->getName() << "���� �������� " << damage << "�� ����!\n";
    bool alive = monster->setHP(monster->getHP() - damage);
    if (alive) cout << "���� HP: " << monster->getHP() << "\n";
    else       cout << "���Ͱ� ���������ϴ�! �¸�!\n";
}
void Archer::attack(Monster* monster) {
    if (!monster) { cout << "���� ����� �����ϴ�.\n"; return; }
    int base = max(1, getPower() - monster->getDefence());
    int per = max(1, base / 3);
    cout << "* �ü� 3���� (Ÿ�� " << per << ")\n";
    for (int i = 1; i <= 3 && monster->getHP() > 0; ++i) {
        cout << "  - [" << i << "] " << monster->getName() << "���� " << per << "�� ����!\n";
        bool alive = monster->setHP(monster->getHP() - per);
        if (!alive) { cout << "���Ͱ� ���������ϴ�! �¸�!\n"; break; }
    }
    if (monster->getHP() > 0) cout << "���� HP: " << monster->getHP() << "\n";
}
void Thief::attack(Monster* monster) {
    if (!monster) { cout << "���� ����� �����ϴ�.\n"; return; }
    int base = max(1, getPower() - monster->getDefence());
    int per = max(1, base / 5);
    cout << "* ���� 5��Ÿ (Ÿ�� " << per << ")\n";
    for (int i = 1; i <= 5 && monster->getHP() > 0; ++i) {
        cout << "  - [" << i << "] " << monster->getName() << "���� " << per << "�� ����!\n";
        bool alive = monster->setHP(monster->getHP() - per);
        if (!alive) { cout << "���Ͱ� ���������ϴ�! �¸�!\n"; break; }
    }
    if (monster->getHP() > 0) cout << "���� HP: " << monster->getHP() << "\n";
}

// ============ Controller ============
enum class Command { None = 0, RecoverHP = 1, RecoverMP = 2, BoostHP = 3, BoostMP = 4, Skill = 5, Ult = 6, Attack = 7, Quit = 8 };

class PlayerController : public Actor {
public:
    Command GetCommand() {
        cout << "��ȣ�� �������ּ��� : ";
        int n;
        if (!(cin >> n)) { ClearInput(); cout << "������ �Է��ϼ���.\n"; return Command::None; }
        if (n < 1 || n > 8) { cout << "1~8 �߿��� �����ϼ���.\n"; return Command::None; }
        return static_cast<Command>(n);
    }
};

// ============ GameMode ============
class GameMode : public Actor {
public:
    void PrintMenu() const {
        cout << "=============================================\n";
        cout << "< ����/���� �ý���(����ǳ) >\n"
            << "1. HP ȸ��(+20, HP����-1)\n"
            << "2. MP ȸ��(+20, MP����-1)\n"
            << "3. HP ��ȭ(x2)\n"
            << "4. MP ��ȭ(x2)\n"
            << "5. ���� ��ų(MP -50, ���� ����)\n"
            << "6. �ʻ��(MP 50% �Ҹ�, ���� ����)\n"
            << "7. ����(�Ϲ�)\n"
            << "8. ������\n";
    }

    void InitStatsInteractive(Player& P, GameState& GS) const {
        int hp, mp, atk, def;
        while (true) {
            cout << "�ʱ� HP�� MP�� �Է��ϼ���(HP/MP�� 50 �ʰ�): ";
            if (!(cin >> hp >> mp)) { ClearInput(); cout << "������ �Է��ϼ���.\n"; continue; }
            if (hp <= MIN_HP_MP || mp <= MIN_HP_MP) { cout << "HP/MP�� �ʹ� �۽��ϴ�. �ٽ�.\n"; continue; }
            break;
        }
        while (true) {
            cout << "�ʱ� ATK�� DEF�� �Է��ϼ���(�� �� 0 �ʰ�): ";
            if (!(cin >> atk >> def)) { ClearInput(); cout << "������ �Է��ϼ���.\n"; continue; }
            if (atk <= MIN_ATK_DEF || def <= MIN_ATK_DEF) { cout << "ATK/DEF�� 0���� Ŀ�� �մϴ�. �ٽ�.\n"; continue; }
            break;
        }
        P.setHP(hp); P.setMP(mp); P.setPower(atk); P.setDefence(def);

        setPotion(5, &GS.HPpots, &GS.MPpots);
        P.printPlayerStatus();
    }

    void Render(const Player& P, const Monster& M, const GameState& GS) const {
        cout << "\n[�÷��̾�] " << P.getNickname() << " (" << P.getJobName() << ") "
            << "HP:" << P.getHP() << " MP:" << P.getMP()
            << " ATK:" << P.getPower() << " DEF:" << P.getDefence()
            << " | ����(HP:" << GS.HPpots << ", MP:" << GS.MPpots << ")\n";
        cout << "[����] " << M.getName()
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
            if (GS.HPpots <= 0) { cout << "HP ������ �����մϴ�.\n"; break; }
            P.setHP(P.getHP() + RECOVER_HP); --GS.HPpots;
            cout << "HP +" << RECOVER_HP << " (HP���� -1)\n";
            break;

        case Command::RecoverMP:
            if (GS.MPpots <= 0) { cout << "MP ������ �����մϴ�.\n"; break; }
            P.setMP(P.getMP() + RECOVER_MP); --GS.MPpots;
            cout << "MP +" << RECOVER_MP << " (MP���� -1)\n";
            break;

        case Command::BoostHP:
            P.setHP(max(1, P.getHP() * 2));
            cout << "HP�� 2��� �����Ǿ����ϴ�. ���� HP: " << P.getHP() << "\n";
            break;

        case Command::BoostMP:
            P.setMP(max(1, P.getMP() * 2));
            cout << "MP�� 2��� �����Ǿ����ϴ�. ���� MP: " << P.getMP() << "\n";
            break;

        case Command::Skill:
            if (P.getMP() < SKILL_MP_COST) { cout << "��ų ��� �Ұ�(MP ����).\n"; break; }
            P.setMP(P.getMP() - SKILL_MP_COST);
            cout << "* ��ų ���: MP -" << SKILL_MP_COST << "\n";
            P.attack(M);
            playerDidAttack = true;
            break;

        case Command::Ult:
            if (P.getMP() <= 0) { cout << "�ʻ�� ��� �Ұ�(MP 0).\n"; break; }
            P.setMP(P.getMP() / 2);
            cout << "* �ʻ��: MP 50% �Ҹ�. ���� MP: " << P.getMP() << "\n";
            P.attack(M);
            playerDidAttack = true;
            break;

        case Command::Attack:
            P.attack(M);
            playerDidAttack = true;
            break;

        case Command::Quit:
            cout << "���α׷��� ���� �մϴ�.\n";
            running = false;
            return;

        default: break;
        }


        if (M->getHP() <= 0) {
            ++GS.wave;
            M = world.SpawnActor<Monster>("������");
            cout << "\n=== �� ���Ͱ� ��Ÿ����! (Wave " << GS.wave << ") ===\n";
            skipCounterThisTurn = true;
        }

        if (!skipCounterThisTurn && M->getHP() > 0) {
            M->attack(&P);
        }

        if (P.getHP() <= 0) {
            cout << "���� ����.\n";
            running = false;
        }
    }
};

// ============ �޴� ��� ============
void printMenu() {
    cout << "=============================================\n";
    cout << "< ����/���� �ý���(����ǳ) >\n"
        << "1. HP ȸ�� (+20, HP���� -1)\n"
        << "2. MP ȸ�� (+20, MP���� -1)\n"
        << "3. HP ��ȭ (x2)\n"
        << "4. MP ��ȭ (x2)\n"
        << "5. ���� ��ų (MP -50��)\n"
        << "6. �ʻ�� (MP 50% �Ҹ�)\n"
        << "7. ���� (���� Ÿ��)\n"
        << "8. ������\n";
}

// ============ main ============
int main() {
    World world;
    GameMode* gm = world.SpawnActor<GameMode>();
    PlayerController* pc = world.SpawnActor<PlayerController>();

    // �÷��̾� ����
    cout << "* �г����� �Է�: ";
    string nickname; cin >> nickname;

    cout << "* ���� ����: 1.���� 2.������ 3.�ü� 4.����\n����: ";
    int job_choice = 0; cin >> job_choice;

    Player* player = nullptr;
    switch (job_choice) {
    case 1: player = world.SpawnActor<Warrior>(nickname);  break;
    case 2: player = world.SpawnActor<Magician>(nickname); break;
    case 3: player = world.SpawnActor<Archer>(nickname);   break;
    case 4: player = world.SpawnActor<Thief>(nickname);    break;
    default: player = world.SpawnActor<Warrior>(nickname); break;
    }

    // �ʱ� ����/���� ����
    gm->InitStatsInteractive(*player, world.GetGameState());

    // ���� ����
    Monster* monster = world.SpawnActor<Monster>("������");

    // ����
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
