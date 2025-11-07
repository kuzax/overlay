#pragma once
#include <includes.h>
#include <sdk.h>
#include <vector>
#include <string>

#define M_PI	3.14159265358979323844f

class c_aimbot {
private:
    struct {
        CClientPlayer* i_target_player;
        CVector vec_crosshair;
        CVector vec_target_bone;
        int i_target_bone;
        bool is_target_player;
        bool is_player_aiming;
    } s_aimbot_config;    
    


    std::vector<std::string> friends_list;  

public:
    struct {
        CClientPlayer* i_target_player;
        CVector vec_crosshair;
        CVector vec_target_bone;
        int i_target_bone;
        bool is_target_player;
        bool is_player_aiming;
    } s_silent_config;       
    
    struct {
        CClientPlayer* i_target_player;
        CVector        vec_target_pos;

    } s_target_config;
    
    struct {
        CClientPlayer* i_target_player;
        CVector vec_crosshair;
        CVector vec_target_bone;
        int i_target_bone;
        bool is_active;
    } s_bullettrack_silent_config;

    void draw_bullettrack_silent_player();
    void draw_bullettrack_fov();

    struct {
        CClientPlayer* i_target_player;
        CVector vec_crosshair;
        CVector vec_target_bone;
        int i_target_bone;
        bool is_active;
    } s_spinbot_silent_config;


    struct {
        CClientPlayer* i_target_player;
        CVector vec_crosshair;
        CVector vec_target_bone;
        int i_target_bone;
        bool is_target_player;
        bool is_player_aiming;
    } s_manual_silent_config;

    struct {
        CClientPlayer* i_target_player;
        CVector vec_crosshair;
        CVector vec_target_bone;
        int i_target_bone;
        bool is_target_player;
        bool is_player_aiming;
    } s_crosshair_config;

    void draw_player_aimbot();
    void draw_aimbot_fov();    
    void draw_crosshair_fov();    
    void draw_crosshair_target();
    void  update_crosshair_position();
    void draw_target_player();
    void draw_silent_player();
    void select_target_player();
    void draw_aimbot();
    void draw_silent();    

    void draw_manual_silent_player();
    void draw_spinbot_silent_player();
    void draw_spinbot_fov();
    void draw_manual_silent();


    void draw_trigger_bot();
    bool is_friend(const std::string& player_name);
    void add_friend(const std::string& player_name);
    void remove_friend(const std::string& player_name);
    float get_distance(CVector vec1, CVector vec2);
};

inline c_aimbot* aimbot = new c_aimbot();