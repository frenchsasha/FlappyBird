#include<string>
#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include "olcConsoleGameEngine.h"

using namespace std;

struct barrier {
    int position;
    int space;
    int levelpos;
};

struct node {
    barrier barrier;
    node* next;
};

class MyLinkedList {
private:
    node* head, * tail;

public:
    MyLinkedList() {
        head = NULL;
        tail = NULL;
    }
    ~MyLinkedList() {

    }

    void add_node(int pos, int spa, int lpos) {
        node* tmp = new node;
        tmp->barrier.position = pos;
        tmp->barrier.space = spa;
        tmp->barrier.levelpos = lpos;
        tmp->next = NULL;

        if (head == NULL) {
            head = tmp;
            tail = tmp;
        }
        else {
            tail->next = tmp;
            tail = tail->next;
        }
    }

    void pop_node() {
        if (head) {
            node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    int list_size() {
        int count = 0;
        node* iter = head;
        while (iter != NULL) {
            count++;
            iter = iter->next;
        }
        return count;
    }

    node get(int index) {
        node* iter = head;
        int count = 0;
        while (head != NULL) {
            if (count == index) {
                return *iter;
            }
            iter = iter->next;
            count++;
        }
    }

    void clear() {
        // node* iter = head;
        while (head) {
            pop_node();
        }
    }
};

class OneLoneCoder_FlappyBird : public olcConsoleGameEngine {
public:
    OneLoneCoder_FlappyBird() {
        m_sAppName = L"Flappy Bird";
    }
   // MyLinkedList Map;

private:
    float fBirdPosition = 0.0f;
    float fBirdVelocity = 0.0f;
    float fBirdAcceleration = 0.0f;

    float fGravity = 100.0f;

    float fSectionWidth;
  
    float fLevelPosition = 0.0f;
    MyLinkedList Map;
    bool bHasCollided = false;
    bool bResetGame = false;
    int nAttemptCount = 0;
    int nFlapCount = 0;
    int nMaxFlapCount = 0;

    int nInitialDist = 15;

protected:
    virtual bool OnUserCreate() {
       // MyLinkedList Map;
        for (int i = 0; i < 4; i++) {
            Map.add_node(0, 0, 0);
        }
        
       // listSection = { {0,0},{0,0},{0,0},{0,0} };
        fSectionWidth = (float)ScreenWidth() / (float)(Map.list_size() - 1);
        bResetGame = true;
        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) {
        if (bResetGame) {
            Map.clear();
            for (int i = 0; i < 4; i++) {
                Map.add_node(0, 0, 0);
            }
            fSectionWidth = (float)ScreenWidth() / (float)(Map.list_size() - 1);
            bHasCollided = false;
            bResetGame = false;
           // listSection = { {0,0},{0,0},{0,0},{0,0} };
            fBirdAcceleration = 0.0f;
            fBirdVelocity = 0.0f;
            fBirdPosition = ScreenHeight() / 2.0f;
            nFlapCount = 0;
            nAttemptCount++;
        }

        fSectionWidth = (float)ScreenWidth() / (float)(Map.list_size() - 1);
        int mapsize = Map.list_size();
        if (bHasCollided) {
            if (m_keys[VK_SPACE].bReleased)
                bResetGame = true;
        }
        else {

            if (m_keys[VK_SPACE].bPressed && fBirdVelocity >= fGravity / 10.0f) {
                fBirdAcceleration = 0.0f;
                fBirdVelocity = -fGravity / 4.0f;

                nFlapCount++;
                
                if (nFlapCount > nMaxFlapCount)
                    nMaxFlapCount= nFlapCount;
            }
            else {
                fBirdAcceleration += fGravity * fElapsedTime;
            }

            if (fBirdAcceleration >= fGravity)
                fBirdAcceleration = fGravity;

            fBirdVelocity += fBirdAcceleration * fElapsedTime;
            fBirdPosition += fBirdVelocity * fElapsedTime;

            fLevelPosition += 14.0f * fElapsedTime;

            if (fLevelPosition>fSectionWidth) {
              //  fLevelPosition -= fSectionWidth;
                //listSection.pop_front();
               // Map.pop_node();


                int position = rand() % (ScreenHeight() - 20);
                int space = rand() % (15) + 20; //from y to (x+y)
                int levelpos = Map.get(Map.list_size() - 1).barrier.levelpos + nInitialDist;// *(4 / mapsize);
                if (position <= 10)position = 0;
                Map.add_node(position,space,levelpos);
            }

            Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
            //int nSection = 0;
            for (int i = 0; i < Map.list_size(); i++) {
                if (Map.get(i).barrier.position !=0) {
                    //DRAW BOTTOM OBSTACLES
                  // Fill(i * fSectionWidth + 10 - fLevelPosition, ScreenHeight() - Map.get(i).barrier.position,
                  //     i * fSectionWidth + 15 - fLevelPosition, ScreenHeight(),
                  //     PIXEL_SOLID, FG_GREEN);
                  // //DRAW TOP
                  // Fill(i * fSectionWidth + 10 - fLevelPosition, 0,
                  //     i * fSectionWidth + 15 - fLevelPosition, ScreenHeight() - Map.get(i).barrier.position - Map.get(i).barrier.space,
                  //     PIXEL_SOLID, FG_GREEN);
                    Fill(Map.get(i).barrier.levelpos - fLevelPosition+ScreenWidth(), ScreenHeight() - Map.get(i).barrier.position,
                        Map.get(i).barrier.levelpos+5 - fLevelPosition + ScreenWidth(), ScreenHeight(),
                        PIXEL_SOLID, FG_GREEN);
                    //DRAW TOP
                    Fill(Map.get(i).barrier.levelpos - fLevelPosition + ScreenWidth(), 0,
                        Map.get(i).barrier.levelpos+5 - fLevelPosition + ScreenWidth(), ScreenHeight() - Map.get(i).barrier.position - Map.get(i).barrier.space,
                        PIXEL_SOLID, FG_GREEN);
                }
              //  nSection++;
            }
            int nBirdX = (int)(ScreenWidth() / 3.0f);

            bHasCollided = fBirdPosition<2 || fBirdPosition>ScreenHeight() - 2 ||
                m_bufScreen[(int)(fBirdPosition + 0) * ScreenWidth() + nBirdX].Char.UnicodeChar != L' ' ||
                m_bufScreen[(int)(fBirdPosition + 1) * ScreenWidth() + nBirdX].Char.UnicodeChar != L' ' ||
                m_bufScreen[(int)(fBirdPosition + 0) * ScreenWidth() + nBirdX + 6].Char.UnicodeChar != L' ' ||
                m_bufScreen[(int)(fBirdPosition + 1) * ScreenWidth() + nBirdX + 6].Char.UnicodeChar != L' ';
   

            if (fBirdVelocity > 0) {
                DrawString(nBirdX, fBirdPosition + 0, L"\\\\\\");
                DrawString(nBirdX, fBirdPosition + 1.0f, L"<\\\\\\=Q");
            }
            else {
                DrawString(nBirdX, fBirdPosition + 2.0f, L"<///=Q");
                DrawString(nBirdX, fBirdPosition + 3.0f, L"<///");
            }

            DrawString(1, 1, L"Attempt: " + to_wstring(nAttemptCount) + L" Score: " + to_wstring(nFlapCount) + L" High Score: " + to_wstring(nMaxFlapCount) 
                + L" FWidth: " + to_wstring(fSectionWidth) + L" ListSize: " + to_wstring(Map.list_size()));

            return true;
        }
    }
};


int main()
{
    OneLoneCoder_FlappyBird game;
    game.ConstructConsole(80, 48, 16, 16);
    game.Start();
    
    return 0;
}

