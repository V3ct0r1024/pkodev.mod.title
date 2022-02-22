#include <Windows.h>
#include <detours.h>

#include <cstdio>
#include <cstring>
#include <string>

#include "loader.h"


namespace pkodev
{
    namespace address
    {
        // Game.exe 0 (1222073761)
        namespace GAME_13X_0
        {
            // inline void SetPreName ( const int n_item, char* sz_name, DWORD& dw_color )
            const unsigned int SetPreName = 0x004A1DC0;

            // inline CItemRecord* GetItemRecordInfo( int nTypeID )
            const unsigned int GetItemRecordInfo = 0x0040CE90;

            // void CHeadSay::Render(D3DXVECTOR3& pos)
            const unsigned int CHeadSay__Render = 0x00470690;
        }

        // Game.exe 1 (1243412597)
        namespace GAME_13X_1
        {
            // inline void SetPreName ( const int n_item, char* sz_name, DWORD& dw_color )
            const unsigned int SetPreName = 0x004A1ED0;

            // inline CItemRecord* GetItemRecordInfo( int nTypeID )
            const unsigned int GetItemRecordInfo = 0x0040CE90;

            // void CHeadSay::Render(D3DXVECTOR3& pos)
            const unsigned int CHeadSay__Render = 0x004706D0;
        }

        // Game.exe 2 (1252912474)
        namespace GAME_13X_2
        {
            // inline void SetPreName ( const int n_item, char* sz_name, DWORD& dw_color )
            const unsigned int SetPreName = 0x004A1F70;

            // inline CItemRecord* GetItemRecordInfo( int nTypeID )
            const unsigned int GetItemRecordInfo = 0x0040CE90;

            // void CHeadSay::Render(D3DXVECTOR3& pos)
            const unsigned int CHeadSay__Render = 0x00470770;
        }

        // Game.exe 3 (1244511158)
        namespace GAME_13X_3
        {
            // inline void SetPreName ( const int n_item, char* sz_name, DWORD& dw_color )
            const unsigned int SetPreName = 0x004A1F60;

            // inline CItemRecord* GetItemRecordInfo( int nTypeID )
            const unsigned int GetItemRecordInfo = 0x0040CE90;

            // void CHeadSay::Render(D3DXVECTOR3& pos)
            const unsigned int CHeadSay__Render = 0x00470760;
        }

        // Game.exe 4 (1585009030)
        namespace GAME_13X_4
        {
            // inline void SetPreName ( const int n_item, char* sz_name, DWORD& dw_color )
            const unsigned int SetPreName = 0x004A2170;

            // inline CItemRecord* GetItemRecordInfo( int nTypeID )
            const unsigned int GetItemRecordInfo = 0x0040CF90;

            // void CHeadSay::Render(D3DXVECTOR3& pos)
            const unsigned int CHeadSay__Render = 0x004707D0;
        }

        // Game.exe 5 (1207214236)
        namespace GAME_13X_5
        {
            // inline void SetPreName ( const int n_item, char* sz_name, DWORD& dw_color )
            const unsigned int SetPreName = 0x004A1BC0;

            // inline CItemRecord* GetItemRecordInfo( int nTypeID )
            const unsigned int GetItemRecordInfo = 0x0040CE80;

            // void CHeadSay::Render(D3DXVECTOR3& pos)
            const unsigned int CHeadSay__Render = 0x00470360;
        }
    }

    namespace pointer
    {
        // inline void SetPreName ( const int n_item, char* sz_name, DWORD& dw_color )
        typedef void(__cdecl* SetPreName__Ptr)(const int, char*, unsigned int&);
        SetPreName__Ptr SetPreName = (SetPreName__Ptr)(void*)(pkodev::address::MOD_EXE_VERSION::SetPreName);

        // inline CItemRecord* GetItemRecordInfo( int nTypeID )
        typedef void* (__cdecl* GetItemRecordInfo__Ptr)(int);
        GetItemRecordInfo__Ptr GetItemRecordInfo = (GetItemRecordInfo__Ptr)(void*)(address::MOD_EXE_VERSION::GetItemRecordInfo);

        // void CHeadSay::Render(D3DXVECTOR3& pos)
        typedef void(__thiscall* CHeadSay__Render__Ptr)(void*, void*);
        CHeadSay__Render__Ptr CHeadSay__Render = (CHeadSay__Render__Ptr)(void*)(address::MOD_EXE_VERSION::CHeadSay__Render);
    }

    namespace hook
    {
        // inline void SetPreName ( const int n_item, char* sz_name, DWORD& dw_color )
        void SetPreName(const int n_item, char* sz_name, unsigned int& dw_color);

        // void CHeadSay::Render(D3DXVECTOR3& pos)
        void __fastcall CHeadSay__Render(void* This, void* NotUsed, void* Pos);
    }
}

// Entry point
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    // Nothing to do . . .
    return TRUE;
}

// Get mod information
void GetModInformation(mod_info& info)
{
    strcpy_s(info.name, TOSTRING(MOD_NAME));
    strcpy_s(info.version, TOSTRING(MOD_VERSION));
    strcpy_s(info.author, TOSTRING(MOD_AUTHOR));
    info.exe_version = MOD_EXE_VERSION;
}

// Start the mod
void Start(const char* path)
{
    // Enable hooks
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)pkodev::pointer::SetPreName, pkodev::hook::SetPreName);
    DetourAttach(&(PVOID&)pkodev::pointer::CHeadSay__Render, pkodev::hook::CHeadSay__Render);
    DetourTransactionCommit();
}

// Stop the mod
void Stop()
{
    // Disable hooks
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)pkodev::pointer::SetPreName, pkodev::hook::SetPreName);
    DetourDetach(&(PVOID&)pkodev::pointer::CHeadSay__Render, pkodev::hook::CHeadSay__Render);
    DetourTransactionCommit();
}

// inline void SetPreName( const int n_item, char* sz_name, DWORD& dw_color )
void pkodev::hook::SetPreName(const int n_item, char* sz_name, unsigned int& dw_color)
{
    // Check item id
    if (n_item != 0)
    {
        // Get the item info
        void* record = pkodev::pointer::GetItemRecordInfo(n_item);

        // Check that the item exists in the ItemInfo
        if (record != nullptr)
        {
            // Get item's type
            const unsigned short int type = *reinterpret_cast<unsigned short int*>(reinterpret_cast<unsigned int>(record) + 0x0134);

            // Check that it is necklace
            if (type == 25)
            {
                // Get title and color
                const char* rgb = reinterpret_cast<const char*>(reinterpret_cast<unsigned int>(record) + 0x00E4);
                const char* title = reinterpret_cast<const char*>(reinterpret_cast<unsigned int>(record) + 0x0258);

                // Convert string with the rgb color to the integer
                const unsigned long color = strtoul(rgb, nullptr, 16);

                // Check that color is not equal to 0 and title is specified
                if (color != 0 && strlen(title) > 0)
                {
                    // Set color
                    dw_color = color;

                    // Set title
                    snprintf(sz_name, 16, "%s", title);

                    // Exit from the hook
                    return;
                }
            }
        }
    }

    // Call original function SetPreName()
    pkodev::pointer::SetPreName(n_item, sz_name, dw_color);
}

// void CHeadSay::Render(D3DXVECTOR3& pos)
void __fastcall pkodev::hook::CHeadSay__Render(void* This, void* NotUsed, void* Pos)
{
    // Pointer to current character on the scene
    void* pCha = reinterpret_cast<void*> (
        *reinterpret_cast<unsigned int*>(
            reinterpret_cast<unsigned int>(This) + 0x28
        )
    );

    // Get character type (player, monster, npc and etc)
    const unsigned int type = *reinterpret_cast<unsigned int*>(
        reinterpret_cast<unsigned int>(pCha) + 0x0CD8
    );

    // Check that character is a player
    if (type == 1)
    {
        // Name color
        unsigned int color = 0xFFFFFFFF;

        // Get necklace ID
        unsigned short int neck_id = *reinterpret_cast<unsigned short int*>(
            reinterpret_cast<unsigned int>(pCha) + 0x095E
        );

        // Check the ID
        if (neck_id != 0)
        {
            // Get necklace item info
            void* item = pkodev::pointer::GetItemRecordInfo(neck_id);

            // Check that item is found
            if (item != nullptr)
            {
                // Get character name color
                const char* szColorName = reinterpret_cast<const char*>(
                    reinterpret_cast<unsigned int>(item) + 0xF7
                );

                // Check tha color is defined
                if (std::strcmp(szColorName, "0") != 0)
                {
                    // Set character's name color
                    color = std::strtoul(szColorName, nullptr, 16);
                }
            }
        }

        // Set character's name color
        *reinterpret_cast<unsigned int*>(
            reinterpret_cast<unsigned int>(This) + 0x38
        ) = color;
    }

    // Call original method CHeadSay::Render(D3DXVECTOR3& pos)
    pkodev::pointer::CHeadSay__Render(This, Pos);
}