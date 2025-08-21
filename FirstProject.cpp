#include <iostream>
#include <vector>

using namespace std;

int main() {
    int hp, mp, attack, defense;
    int hpPotions = 5;
    int mpPotions = 5;
    int choice;

    // �ʱ� ���� �Է�
    cout << "HP�� MP�� �Է����ּ���: ";
    cin >> hp >> mp;
    cout << "���ݷ°� ������ �Է����ּ���: ";
    cin >> attack >> defense;

    cout << "* ������ ���޵Ǿ����ϴ�. (HP, MP ���� �� 5��)" << endl;

    while (true) {
        cout << "=============================================" << endl;
        cout << "<���� ���� �ý���>" << endl;
        cout << "1. HP ȸ��" << endl;
        cout << "2. MP ȸ��" << endl;
        cout << "3. HP ��ȭ" << endl;
        cout << "4. MP ��ȭ" << endl;
        cout << "5. ���� ��ų ���" << endl;
        if (hpPotions == 0 && mpPotions == 0) {
            cout << "6. �ʻ�� ���" << endl;
            cout << "7. ������" << endl;
        }
        cout << "��ȣ�� �������ּ���: ";
        cin >> choice;

        switch (choice) {
        case 1:
            if (hpPotions > 0) {
                hp += 20;
                hpPotions--;
                cout << "* HP�� 20 ȸ���Ǿ����ϴ�. ������ 1�� �����˴ϴ�." << endl;
                cout << "���� HP: " << hp << " ���� ���� ��: " << hpPotions << endl;
            }
            else {
                cout << "HP ������ �����մϴ�." << endl;
            }
            break;
        case 2:
            if (mpPotions > 0) {
                mp += 20;
                mpPotions--;
                cout << "* MP�� 20 ȸ���Ǿ����ϴ�. ������ 1�� �����˴ϴ�." << endl;
                cout << "���� MP: " << mp << " ���� ���� ��: " << mpPotions << endl;
            }
            else {
                cout << "MP ������ �����մϴ�." << endl;
            }
            break;
        case 3:
            hp *= 2;
            cout << "* HP�� 2��� �����Ǿ����ϴ�. ���� HP: " << hp << endl;
            break;
        case 4:

            mp *= 2;
            cout << "* MP�� 2��� �����Ǿ����ϴ�. ���� MP: " << mp << endl;
            break;

        case 5:
            if (mp >= 50) {
                mp -= 50;
                cout << "* ��ų�� ����Ͽ� MP�� 50 �Ҹ�Ǿ����ϴ�. ���� MP: " << mp << endl;
            }
            else {
                cout << "MP�� �����Ͽ� ��ų�� ����� �� �����ϴ�." << endl;
            }
            break;

        case 6:
            
            if (hpPotions == 0 && mpPotions == 0) {
                cout << "�ʻ�� ���" << endl;
                // �ʻ�� ���� �߰�
            }

            else {
                cout << "������ �����־� �ʻ�⸦ ����� �� �����ϴ�." << endl;
            }

            break;
        case 7:
            cout << "������ �����մϴ�." << endl;
            return 0;

        default:
            cout << "�߸��� ��ȣ�Դϴ�. �ٽ� �������ּ���." << endl;
        }
    }

    return 0;
}