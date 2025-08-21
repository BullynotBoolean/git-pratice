#include <iostream>
#include <vector>

using namespace std;

int main() {
    int hp, mp, attack, defense;
    int hpPotions = 5;
    int mpPotions = 5;
    int choice;

    // 초기 스탯 입력
    cout << "HP와 MP를 입력해주세요: ";
    cin >> hp >> mp;
    cout << "공격력과 방어력을 입력해주세요: ";
    cin >> attack >> defense;

    cout << "* 포션이 지급되었습니다. (HP, MP 포션 각 5개)" << endl;

    while (true) {
        cout << "=============================================" << endl;
        cout << "<스탯 관리 시스템>" << endl;
        cout << "1. HP 회복" << endl;
        cout << "2. MP 회복" << endl;
        cout << "3. HP 강화" << endl;
        cout << "4. MP 강화" << endl;
        cout << "5. 공격 스킬 사용" << endl;
        if (hpPotions == 0 && mpPotions == 0) {
            cout << "6. 필살기 사용" << endl;
            cout << "7. 나가기" << endl;
        }
        cout << "번호를 선택해주세요: ";
        cin >> choice;

        switch (choice) {
        case 1:
            if (hpPotions > 0) {
                hp += 20;
                hpPotions--;
                cout << "* HP가 20 회복되었습니다. 포션이 1개 차감됩니다." << endl;
                cout << "현재 HP: " << hp << " 남은 포션 수: " << hpPotions << endl;
            }
            else {
                cout << "HP 포션이 부족합니다." << endl;
            }
            break;
        case 2:
            if (mpPotions > 0) {
                mp += 20;
                mpPotions--;
                cout << "* MP가 20 회복되었습니다. 포션이 1개 차감됩니다." << endl;
                cout << "현재 MP: " << mp << " 남은 포션 수: " << mpPotions << endl;
            }
            else {
                cout << "MP 포션이 부족합니다." << endl;
            }
            break;
        case 3:
            hp *= 2;
            cout << "* HP가 2배로 증가되었습니다. 현재 HP: " << hp << endl;
            break;
        case 4:

            mp *= 2;
            cout << "* MP가 2배로 증가되었습니다. 현재 MP: " << mp << endl;
            break;

        case 5:
            if (mp >= 50) {
                mp -= 50;
                cout << "* 스킬을 사용하여 MP가 50 소모되었습니다. 현재 MP: " << mp << endl;
            }
            else {
                cout << "MP가 부족하여 스킬을 사용할 수 없습니다." << endl;
            }
            break;

        case 6:
            
            if (hpPotions == 0 && mpPotions == 0) {
                cout << "필살기 사용" << endl;
                // 필살기 로직 추가
            }

            else {
                cout << "포션이 남아있어 필살기를 사용할 수 없습니다." << endl;
            }

            break;
        case 7:
            cout << "게임을 종료합니다." << endl;
            return 0;

        default:
            cout << "잘못된 번호입니다. 다시 선택해주세요." << endl;
        }
    }

    return 0;
}