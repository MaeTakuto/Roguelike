#include "../dxlib_ext.h"
#include "dxlib_ext_particle.h"

namespace dxe {

    //---------------------------------------------------------------------------------------------------------------------------------------------
    void Particle::drawGuiController(const tnl::Vector2i& pos) {

        if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F1)) start();
        if (tnl::Input::IsKeyDownTrigger(eKeys::KB_F2)) stop();

        int x = pos.x;
        int y = pos.y;

        tnl::Vector3 ms = tnl::Input::GetMousePosition();
        int tx = x + 330;
        int ty = y;
        int bx = tx + 250;
        int by = y + 30;

        description_str_.clear();
        description_str_ = "[ F1 : 生成開始 start() ] [ F2 : 停止 stop() ]";
        is_btn_save_forcus_ = false;
        if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
            is_btn_save_forcus_ = true;
            if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
                is_btn_save_select_ = true;
            }
            description_str_ = "particle.bin の名前でプロジェクトのルートディレクトリに保存されます";
        }
        if (is_btn_save_select_) {
            if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
                saveStatus("particle.bin");
                is_btn_save_select_ = false;
            }
        }

        int color = (is_btn_save_forcus_) ? -1 : 0x88888888;
        color = (is_btn_save_select_) ? 0xff00ff00 : color;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(tx, ty, bx, by, 0, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
        DrawBox(tx, ty, bx, by, color, false);
        DrawStringEx((float)tx + 5, (float)ty + 5, color, "save to file");


        if (!gui_drag_resouce_) {
            gui_drag_resouce_ = Shared<GuiDragBox>(new GuiDragBox([&](const std::string& file_path) {
                auto path = tnl::DetachmentFilePath(file_path);
                if (std::get<2>(path) == "bin") {
                    loadStatus(file_path);
                    reloadTexture();
                    refreshProcess();
                    gui_play_type_ = nullptr;
                    gui_blend_mode_ = nullptr;
                    gui_adulation_mode_ = nullptr;
                    gui_origin_mode_ = nullptr;
                    gui_move_surface_ = nullptr;
                    gui_posture_type_ = nullptr;
                    gui_distortion_mode_ = nullptr;
                    gui_disp_size_mode_ = nullptr;
                    gui_disp_alpha_mode_ = nullptr;
                    gui_eject_type_ = nullptr;

                    gui_particle_num_ = nullptr;
                    gui_time_limit_ = nullptr;
                    gui_origin_range_ = nullptr;
                    gui_waver_factor_ = nullptr;
                    gui_start_velocity_ = nullptr;
                    gui_gravity_factor_ = nullptr;
                    gui_position_x_ = nullptr;
                    gui_position_y_ = nullptr;
                    gui_position_z_ = nullptr;
                    gui_conv_position_x_ = nullptr;
                    gui_conv_position_y_ = nullptr;
                    gui_conv_position_z_ = nullptr;
                }
                else {
                    texture_ = dxe::Texture::CreateFromFile(file_path);
                    reloadTexture();
                }
                }));
            gui_drag_resouce_->setLabel("Drag the relevant files");
            gui_drag_resouce_->setPosition({ tx + 250, pos.y + y });
            gui_drag_resouce_->setWidth(250);
            gui_drag_resouce_->setDescription([&]() {
                description_str_ = "テクスチャもしくはセーブデータのバイナリファイルをドラッグしてください";
                });
        }
        gui_drag_resouce_->update();

        int height = 25;
        y += height;
        if (!gui_play_type_) {
            gui_play_type_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setPlayType(static_cast<ePlayType>(select_index));
                    }, "PlayType::LOOP", "PlayType::ONCE"
                ));
            gui_play_type_->setCurrentIndex(static_cast<uint32_t>(play_type_));
            gui_play_type_->setWidth(250);
            gui_play_type_->setPosition({ tx, pos.y + y });
            gui_play_type_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_play_type_->setDescription([&]() {
                description_str_ = "[ LOOP : 時間に対して均等に排出されます ] [ ONCE :１度に全ての粒子が排出され、再度再生命令があるまで排出されません ]";
                });
        }
        y += height;
        if (!gui_blend_mode_) {
            gui_blend_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setBlendMode(static_cast<eBlendState>(select_index));
                    }, "BlendMode::ALPHA", "BlendMode::ADD", "BlendMode::SUB", "BlendMode::MUL"
                ));
            gui_blend_mode_->setCurrentIndex(static_cast<uint32_t>(blend_mode_));
            gui_blend_mode_->setWidth(250);
            gui_blend_mode_->setPosition({ tx, pos.y + y });
            gui_blend_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_blend_mode_->setDescription([&]() {
                description_str_ = "[ ALPHA : 通常の半透明 ] [ ADD : 加算 ] [ SUB : 減算 ] [ MUL : 乗算 ]";
                });

        }

        y += height;
        if (!gui_origin_mode_) {
            gui_origin_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setOriginMode(static_cast<eOriginMode>(select_index));
                    }, "OriginMode::XYZ", "OriginMode::XY", "OriginMode::XZ", "OriginMode::YZ", "OriginMode::AXIS", "OriginMode::SPHERE", "OriginMode::CXY", "OriginMode::CXZ", "OriginMode::CYZ"
                ));
            gui_origin_mode_->setCurrentIndex(static_cast<uint32_t>(origin_mode_));
            gui_origin_mode_->setWidth(250);
            gui_origin_mode_->setPosition({ tx, pos.y + y });
            gui_origin_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_origin_mode_->setDescription([&]() {
                description_str_ = "出現範囲 [ XYZ : 球内範囲 ] [ XY, XZ, YZ : 平面の円内範囲 ] [ AXIS : 軸上 ] [ SPHERE : 球面上 ] [ CXY, CXZ, CYZ 平面の円上 ] OrigineRange で調整";
                });
        }

        y += height;
        if (!gui_eject_type_) {
            gui_eject_type_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setEjectType(static_cast<eEjectType>(select_index));
                    }, "EjectType::DIFF", "EjectType::CONV"
                ));
            gui_eject_type_->setCurrentIndex(static_cast<uint32_t>(eject_type_));
            gui_eject_type_->setWidth(250);
            gui_eject_type_->setPosition({ tx, pos.y + y });
            gui_eject_type_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_eject_type_->setDescription([&]() {
                description_str_ = "発生した粒子が拡散するか収束するか [ DIFF : 拡散 ] [ CONV : 収束 ]";
            }) ;
        }

        y += height;
        if (!gui_move_surface_) {
            gui_move_surface_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setMoveSurface(static_cast<eMoveSurface>(select_index));
                    }, "MoveSurface::XYZ", "MoveSurface::XY", "MoveSurface::XZ", "MoveSurface::YZ"
                ));
            gui_move_surface_->setCurrentIndex(static_cast<uint32_t>(move_surface_));
            gui_move_surface_->setWidth(250);
            gui_move_surface_->setPosition({ tx, pos.y + y });
            gui_move_surface_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_move_surface_->setDescription([&]() {
                description_str_ = "粒子の移動 [ XYZ : 全方位 ] [ XY, XZ, YZ : 平面 ]";
                });
        }

        y += height;
        if (!gui_posture_type_) {
            gui_posture_type_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setPostureType(static_cast<ePostureType>(select_index));
                    }, "PostureType::BXYZ", "PostureType::BY", "PostureType::OBY", "PostureType::UpYD", "PostureType::X", "PostureType::Y", "PostureType::Z"
                ));
            gui_posture_type_->setCurrentIndex(static_cast<uint32_t>(posture_type_));
            gui_posture_type_->setWidth(250);
            gui_posture_type_->setPosition({ tx, pos.y + y });
            gui_posture_type_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_posture_type_->setDescription([&]() {
                description_str_ = "粒子の向き [ BXYZ:ビルボード ] [ BY:Y軸ビルボード ] [ OBY:上向Y軸ビルボード ] [ UpYD:移動方向に沿うYアップ ] [ X, Y, Z:向軸固定 ]";
                });

        }

        y += height;
        if (!gui_adulation_mode_) {
            gui_adulation_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setAdulationEmitter((0 == select_index) ? false : true);
                    }, "AdulationEmitter : FALSE", "AdulationEmitter : TRUE"
                ));
            gui_adulation_mode_->setCurrentIndex(static_cast<uint32_t>((is_adulation_emitter) ? 1 : 0));
            gui_adulation_mode_->setWidth(250);
            gui_adulation_mode_->setPosition({ tx, pos.y + y });
            gui_adulation_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_adulation_mode_->setDescription([&]() {
                description_str_ = "出現後の粒子が Position に追従するか";
                });
        }

        y += height;
        if (!gui_disp_size_mode_) {
            gui_disp_size_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setDispSizeMode(static_cast<eDispSizeMode>(select_index));
                    }, "DispSizeMode::NONE", "DispSizeMode::EXIST", "DispSizeMode::BIGGER", "DispSizeMode::SMALLER"
                ));
            gui_disp_size_mode_->setCurrentIndex(static_cast<uint32_t>(disp_size_mode_));
            gui_disp_size_mode_->setWidth(250);
            gui_disp_size_mode_->setPosition({ tx, pos.y + y });
            gui_disp_size_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_disp_size_mode_->setDescription([&]() {
                description_str_ = "出現後のサイズ変位 [ NONE : なし ] [ EXIST : 出現直後と消える直前にサイズ変化 ] [ BIGGER : 徐々に大きく ] [ SMALLER : 徐々に小さく ]";
                });

        }

        y += height;
        if (!gui_disp_alpha_mode_) {
            gui_disp_alpha_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setDispAlphaMode(static_cast<eDispAlphaMode>(select_index));
                    }, "DispAlphaMode::NONE", "DispAlphaMode::EXIST", "DispAlphaMode::INC", "DispAlphaMode::DEC"
                ));
            gui_disp_alpha_mode_->setCurrentIndex(static_cast<uint32_t>(disp_alpha_mode_));
            gui_disp_alpha_mode_->setWidth(250);
            gui_disp_alpha_mode_->setPosition({ tx, pos.y + y });
            gui_disp_alpha_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_disp_alpha_mode_->setDescription([&]() {
                description_str_ = "出現後の透明度変位 [ NONE : なし ] [ EXIST : 出現直後と消える直前に透明度変化 ] [ INC : 徐々に濃く ] [ DEC : 徐々に薄く ]";
                });

        }

        y += height;
        if (!gui_distortion_mode_) {
            gui_distortion_mode_ = Shared<GuiMenuSelector>(
                new GuiMenuSelector(
                    [&](uint32_t select_index) {
                        setDistortionMode(static_cast<eDistortionMode>(select_index));
                    }, "DistortionMode::CHAOS", "DistortionMode::ORDER"
                ));
            gui_distortion_mode_->setCurrentIndex(static_cast<uint32_t>(distortion_mode_));
            gui_distortion_mode_->setWidth(250);
            gui_distortion_mode_->setPosition({ tx, pos.y + y });
            gui_distortion_mode_->setLocation(dxe::GuiMenuSelector::eLocation::RIGHT_DOWN);
            gui_distortion_mode_->setDescription([&]() {
                description_str_ = "形状変化のモード [ CHAOS : 頂点位置が交差したり混沌とした変化をします ] [ ORDER : 頂点は交差せず秩序立った変化をします ]";
                });
        }

        if (tnl::Input::IsKeyDown(eKeys::KB_LCONTROL) && tnl::Input::IsKeyDownTrigger(eKeys::KB_F12)) {

            y = DXE_WINDOW_HEIGHT - 70 ;
            if (!gui_scale_) {
                gui_scale_ = Shared<GuiValueSlider< Particle, float >>(
                    new GuiValueSlider< Particle, float >
                    (this
                        , &Particle::getScale
                        , &Particle::setScale
                        , { tx, pos.y + y }
                        , 0
                        , 3.0f
                        , "scale"));
                gui_scale_->setDescription([&]() {
                    description_str_ = "全体に影響するスケール disp size factor と合わせて調整してください";
                    });
            }

            height = 20;
            y += height;
            if (!gui_existence_scale_) {
                gui_existence_scale_ = Shared<GuiValueSlider< Particle, float >>(
                    new GuiValueSlider< Particle, float >
                    (this
                        , &Particle::getDispSizeFactor
                        , &Particle::setDispSizeFactor
                        , { tx, pos.y + y }
                        , 0.0f
                        , 3.0f
                        , "disp size factor"));
                gui_existence_scale_->setDescription([&]() {
                    description_str_ = "粒子が出現してからのサイズの変位係数 DispSizeMode の設定と連携します";
                    });
            }

            y += height;
            if (!gui_disp_alpha_factor_) {
                gui_disp_alpha_factor_ = Shared<GuiValueSlider< Particle, float >>(
                    new GuiValueSlider< Particle, float >
                    (this
                        , &Particle::getDispAlphaFactor
                        , &Particle::setDispAlphaFactor
                        , { tx, pos.y + y }
                        , 0
                        , 3.0f
                        , "disp alpha factor"));
                gui_disp_alpha_factor_->setDescription([&]() {
                    description_str_ = "粒子が出現してからの透明度の変位係数 DispAlphaMode の設定と連携します";
                    });
            }
        }


        y = 0;
        if (!gui_particle_num_) {
            gui_particle_num_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::INT,
                    [&](const std::string& input_string) {
                        int num = std::atoi(input_string.c_str());
                        num = std::clamp(num, 5, 10000);
                        setParticleNum(num);
                        gui_particle_num_->setBoxString(std::to_string(getParticleNum()));
                    }, 5));
            gui_particle_num_->setPosition({ pos.x, pos.y + y });
            gui_particle_num_->setBoxString(std::to_string(getParticleNum()));
            gui_particle_num_->setLabel("particle num");
            gui_particle_num_->setSize({ 330, 20 });
            gui_particle_num_->setBoxWidth(101);
            gui_particle_num_->setDescription([&]() {
                description_str_ = "粒子の数 多いほど処理速度に影響を与えるので注意";
                });
        }

        height = 20;
        y += height;
        if (!gui_time_limit_) {
            gui_time_limit_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::FLOAT,
                    [&](const std::string& input_string) {
                        float num = (float)std::atof(input_string.c_str());
                        num = std::clamp(num, 0.05f, 60.0f);
                        setTimeLimit(num);
                        gui_time_limit_->setBoxString(tnl::FloatToString(getTimeLimit(), "%.2f"));
                    }, 5));
            gui_time_limit_->setPosition({ pos.x, pos.y + y });
            gui_time_limit_->setBoxString(tnl::FloatToString(getTimeLimit(), "%.2f"));
            gui_time_limit_->setLabel("time limit");
            gui_time_limit_->setSize({ 330, 20 });
            gui_time_limit_->setBoxWidth(101);
            gui_time_limit_->setDescription([&]() {
                description_str_ = "粒子が生み出されてから消えるまでの制限時間 (秒)";
                });
        }

        y += height;
        if (!gui_origin_range_) {
            gui_origin_range_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::INT,
                    [&](const std::string& input_string) {
                        int num = std::atoi(input_string.c_str());
                        setOriginRange(static_cast<float>(num));
                        gui_origin_range_->setBoxString(tnl::FloatToString(getOriginRange(), "%.0f"));
                    }, 5));
            gui_origin_range_->setPosition({ pos.x, pos.y + y });
            gui_origin_range_->setBoxString(tnl::FloatToString(getOriginRange(), "%.0f"));
            gui_origin_range_->setLabel("origin range");
            gui_origin_range_->setSize({ 330, 20 });
            gui_origin_range_->setBoxWidth(101);
            gui_origin_range_->setDescription([&]() {
                description_str_ = "粒子が発生する範囲 OriginMode で球状や円状にできます";
                });
        }

        y += height;
        if (!gui_waver_factor_) {
            gui_waver_factor_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::INT,
                    [&](const std::string& input_string) {
                        int num = std::atoi(input_string.c_str());
                        setWaverFactor(static_cast<float>(num));
                        gui_waver_factor_->setBoxString(tnl::FloatToString(getWaverFactor(), "%.0f"));
                    }, 5));
            gui_waver_factor_->setPosition({ pos.x, pos.y + y });
            gui_waver_factor_->setBoxString(tnl::FloatToString(getWaverFactor(), "%.0f"));
            gui_waver_factor_->setLabel("waver factor");
            gui_waver_factor_->setSize({ 330, 20 });
            gui_waver_factor_->setBoxWidth(101);
            gui_waver_factor_->setDescription([&]() {
                description_str_ = "移動の揺らぎ係数";
            });
        }

        y += height;
        if (!gui_start_velocity_) {
            gui_start_velocity_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::INT,
                    [&](const std::string& input_string) {
                        int num = std::atoi(input_string.c_str());
                        setStartVelocity(static_cast<float>(num));
                        gui_start_velocity_->setBoxString(tnl::FloatToString(getStartVelocity(), "%.0f"));
                    }, 5));
            gui_start_velocity_->setPosition({ pos.x, pos.y + y });
            gui_start_velocity_->setBoxString(tnl::FloatToString(getStartVelocity(), "%.0f"));
            gui_start_velocity_->setLabel("start vlcty");
            gui_start_velocity_->setSize({ 330, 20 });
            gui_start_velocity_->setBoxWidth(101);
            gui_start_velocity_->setDescription([&]() {
                description_str_ = "direction 方向へ放出される粒子の速度 EjectTypeをDIFFに設定したときのみ有効";
                });
        }

        y += height;
        if (!gui_gravity_factor_) {
            gui_gravity_factor_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::INT,
                    [&](const std::string& input_string) {
                        int num = std::atoi(input_string.c_str());
                        setGravityFactor(static_cast<float>(num));
                        gui_gravity_factor_->setBoxString(tnl::FloatToString(getGravityFactor(), "%.0f"));
                    }, 5));
            gui_gravity_factor_->setPosition({ pos.x, pos.y + y });
            gui_gravity_factor_->setBoxString(tnl::FloatToString(getGravityFactor(), "%.0f"));
            gui_gravity_factor_->setLabel("gravity factor");
            gui_gravity_factor_->setSize({ 330, 20 });
            gui_gravity_factor_->setBoxWidth(101);
            gui_gravity_factor_->setDescription([&]() {
                description_str_ = "重力係数 EjectTypeをDIFFに設定したときのみ有効";
                });
        }

        y += height;
        if (!gui_position_x_) {
            gui_position_x_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::INT,
                    [&](const std::string& input_string) {
                        int num = std::atoi(input_string.c_str());
                        setPosition({ static_cast<float>(num), getPosition().y, getPosition().z });
                        gui_position_x_->setBoxString(tnl::FloatToString(getPosition().x, "%.0f"));
                    }, 5));
            gui_position_x_->setPosition({ pos.x, pos.y + y });
            gui_position_x_->setBoxString(tnl::FloatToString(getPosition().x, "%.0f"));
            gui_position_x_->setLabel("position x");
            gui_position_x_->setSize({ 330, 20 });
            gui_position_x_->setBoxWidth(101);
        }

        y += height;
        if (!gui_position_y_) {
            gui_position_y_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::INT,
                    [&](const std::string& input_string) {
                        int num = std::atoi(input_string.c_str());
                        setPosition({ getPosition().x, static_cast<float>(num), getPosition().z });
                        gui_position_y_->setBoxString(tnl::FloatToString(getPosition().y, "%.0f"));
                    }, 5));
            gui_position_y_->setPosition({ pos.x, pos.y + y });
            gui_position_y_->setBoxString(tnl::FloatToString(getPosition().y, "%.0f"));
            gui_position_y_->setLabel("position y");
            gui_position_y_->setSize({ 330, 20 });
            gui_position_y_->setBoxWidth(101);
        }

        y += height;
        if (!gui_position_z_) {
            gui_position_z_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::INT,
                    [&](const std::string& input_string) {
                        int num = std::atoi(input_string.c_str());
                        setPosition({ getPosition().x, getPosition().y, static_cast<float>(num) });
                        gui_position_z_->setBoxString(tnl::FloatToString(getPosition().z, "%.0f"));
                    }, 5));
            gui_position_z_->setPosition({ pos.x, pos.y + y });
            gui_position_z_->setBoxString(tnl::FloatToString(getPosition().z, "%.0f"));
            gui_position_z_->setLabel("position z");
            gui_position_z_->setSize({ 330, 20 });
            gui_position_z_->setBoxWidth(101);
        }

        y += height;
        if (!gui_conv_position_x_) {
            gui_conv_position_x_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::INT,
                    [&](const std::string& input_string) {
                        int num = std::atoi(input_string.c_str());
                        setConvPosition({ static_cast<float>(num), getConvPosition().y, getConvPosition().z });
                        gui_conv_position_x_->setBoxString(tnl::FloatToString(getConvPosition().x, "%.0f"));
                    }, 5));
            gui_conv_position_x_->setPosition({ pos.x, pos.y + y });
            gui_conv_position_x_->setBoxString(tnl::FloatToString(getConvPosition().x, "%.0f"));
            gui_conv_position_x_->setLabel("conv position x");
            gui_conv_position_x_->setSize({ 330, 20 });
            gui_conv_position_x_->setBoxWidth(101);
            gui_conv_position_x_->setDescription([&]() {
                description_str_ = "粒子の収束座標 EjectTypeをCONVに設定したときのみ有効 この座標はPositionからの相対座標です";
                });
        }

        y += height;
        if (!gui_conv_position_y_) {
            gui_conv_position_y_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::INT,
                    [&](const std::string& input_string) {
                        int num = std::atoi(input_string.c_str());
                        setConvPosition({ getConvPosition().x, static_cast<float>(num), getConvPosition().z });
                        gui_conv_position_y_->setBoxString(tnl::FloatToString(getConvPosition().y, "%.0f"));
                    }, 5));
            gui_conv_position_y_->setPosition({ pos.x, pos.y + y });
            gui_conv_position_y_->setBoxString(tnl::FloatToString(getConvPosition().y, "%.0f"));
            gui_conv_position_y_->setLabel("conv position y");
            gui_conv_position_y_->setSize({ 330, 20 });
            gui_conv_position_y_->setBoxWidth(101);
            gui_conv_position_y_->setDescription([&]() {
                description_str_ = "粒子の収束座標 EjectTypeをCONVに設定したときのみ有効 この座標はPositionからの相対座標です";
                });
        }

        y += height;
        if (!gui_conv_position_z_) {
            gui_conv_position_z_ = Shared<GuiInputBox>(
                new GuiInputBox
                (GuiInputBox::eInputType::INT,
                    [&](const std::string& input_string) {
                        int num = std::atoi(input_string.c_str());
                        setConvPosition({ getConvPosition().x, getConvPosition().y, static_cast<float>(num) });
                        gui_conv_position_z_->setBoxString(tnl::FloatToString(getConvPosition().z, "%.0f"));
                    }, 5));
            gui_conv_position_z_->setPosition({ pos.x, pos.y + y });
            gui_conv_position_z_->setBoxString(tnl::FloatToString(getConvPosition().z, "%.0f"));
            gui_conv_position_z_->setLabel("conv position z");
            gui_conv_position_z_->setSize({ 330, 20 });
            gui_conv_position_z_->setBoxWidth(101);
            gui_conv_position_z_->setDescription([&]() {
                description_str_ = "粒子の収束座標 EjectTypeをCONVに設定したときのみ有効 この座標はPositionからの相対座標です";
                });
        }

        
        y += 30;
        if (!gui_emissive_) {
            gui_emissive_ = Shared<GuiValueSlider< Particle, tnl::Vector3 >>(
                new GuiValueSlider< Particle, tnl::Vector3 >
                (this
                    , &Particle::getEmissive
                    , &Particle::setEmissive
                    , { pos.x, pos.y + y }
                    , { 0, 0, 0 }
                    , { 5.0f, 5.0f, 5.0f }
            , "emissive"));
            gui_emissive_->setDescription([&]() {
                description_str_ = "自己発光色";
            });
        }

        height = 60;
        y += height;
        if (!gui_diff_direction_) {
            gui_diff_direction_ = Shared<GuiValueSlider< Particle, tnl::Vector3 >>(
                new GuiValueSlider< Particle, tnl::Vector3 >
                (this
                    , &Particle::getDiffDirection
                    , &Particle::setDiffDirection
                    , { pos.x, pos.y + y }
                    , { -1.0f, -1.0f, -1.0f }
                    , { 1.0f, 1.0f, 1.0f }
            , "diffusion dir"));
            gui_diff_direction_->setDescription([&]() {
                description_str_ = "拡散方向 EjectTypeをDIFFに設定したときのみ有効";
            });
        }
        y += height;
        if (!gui_gravity_direction_) {
            gui_gravity_direction_ = Shared<GuiValueSlider< Particle, tnl::Vector3 >>(
                new GuiValueSlider< Particle, tnl::Vector3 >
                (this
                    , &Particle::getGravityDirection
                    , &Particle::setGravityDirection
                    , { pos.x, pos.y + y }
                    , { -1.0f, -1.0f, -1.0f }
                    , { 1.0f, 1.0f, 1.0f }
            , "gravity dir"));
            gui_gravity_direction_->setDescription([&]() {
                description_str_ = "重力方向 EjectTypeをDIFFに設定したときのみ有効";
            });
        }
        y += height;
        if (!gui_origin_axis_) {
            gui_origin_axis_ = Shared<GuiValueSlider< Particle, tnl::Vector3 >>(
                new GuiValueSlider< Particle, tnl::Vector3 >
                (this
                    , &Particle::getOriginAxis
                    , &Particle::setOriginAxis
                    , { pos.x, pos.y + y }
                    , { -1.0f, -1.0f, -1.0f }
                    , { 1.0f, 1.0f, 1.0f }
            , "origin axis"));
            gui_origin_axis_->setDescription([&]() {
                description_str_ = "OriginMode を Axis に設定したときに有効な生成軸";
                });
        }


        y += height;
        height = 20;
        if (!gui_time_scale_) {
            gui_time_scale_ = Shared<GuiValueSlider< Particle, float >>(
                new GuiValueSlider< Particle, float >
                (this
                    , &Particle::getTimeScale
                    , &Particle::setTimeScale
                    , { pos.x, pos.y + y }
                    , 0.001f
                    , 3.0f
                    , "time scale"));
            gui_time_scale_->setDescription([&]() {
                description_str_ = "全体の時間スケール 最終的な調整に使用する事を推奨";
                });
        }

        y += height;
        if (!gui_alpha_) {
            gui_alpha_ = Shared<GuiValueSlider< Particle, float >>(
                new GuiValueSlider< Particle, float >
                (this
                    , &Particle::getAlpha
                    , &Particle::setAlpha
                    , { pos.x, pos.y + y }
                    , 0
                    , 1.0f
                    , "alpha"));
            gui_alpha_->setDescription([&]() {
                description_str_ = "全体の透明度";
                });
        }

        y += height;
        if (!gui_near_alpha_distance_) {
            gui_near_alpha_distance_ = Shared<GuiValueSlider< Particle, float >>(
                new GuiValueSlider< Particle, float >
                (this
                    , &Particle::getNearAlphaDistance
                    , &Particle::setNearAlphaDistance
                    , { pos.x, pos.y + y }
                    , 0
                    , 100.0f
                    , "near alpha dist"));
            gui_near_alpha_distance_->setDescription([&]() {
                description_str_ = "カメラに近い場合に透明化する距離";
                });
        }

        y += height;
        if (!gui_size_x_) {
            gui_size_x_ = Shared<GuiValueSlider< Particle, float >>(
                new GuiValueSlider< Particle, float >
                (this
                    , &Particle::getSizeX
                    , &Particle::setSizeX
                    , { pos.x, pos.y + y }
                    , 0
                    , 10.0f
                    , "size x"));
            gui_size_x_->setDescription([&]() {
                description_str_ = "粒子のサイズ X 大きいほど処理速度に影響を与えるので注意";
                });
        }
        y += height;
        if (!gui_size_y_) {
            gui_size_y_ = Shared<GuiValueSlider< Particle, float >>(
                new GuiValueSlider< Particle, float >
                (this
                    , &Particle::getSizeY
                    , &Particle::setSizeY
                    , { pos.x, pos.y + y }
                    , 0
                    , 10.0f
                    , "size y"));
            gui_size_y_->setDescription([&]() {
                description_str_ = "粒子のサイズ Y 大きいほど処理速度に影響を与えるので注意";
            });
        }

        y += height;
        if (!gui_moving_decay_factor_) {
            gui_moving_decay_factor_ = Shared<GuiValueSlider< Particle, float >>(
                new GuiValueSlider< Particle, float >
                (this
                    , &Particle::getMovingDecayFactor
                    , &Particle::setMovingDecayFactor
                    , { pos.x, pos.y + y }
                    , 0.0f
                    , 5.0f
                    , "move decay factor"));
            gui_moving_decay_factor_->setDescription([&]() {
                description_str_ = "放出された粒子の速度の減衰係数 EjectTypeをDIFFに設定したときのみ有効";
                });
        }

        y += height;
        if (!gui_diff_random_angle_range_) {
            gui_diff_random_angle_range_ = Shared<GuiValueSlider< Particle, float >>(
                new GuiValueSlider< Particle, float >
                (this
                    , &Particle::getDiffRandomAngleRange
                    , &Particle::setDiffRandomAngleRange
                    , { pos.x, pos.y + y }
                    , 0.0f
                    , 180.0f
                    , "diffusion range"));
            gui_diff_random_angle_range_->setDescription([&]() {
                description_str_ = "diffusion dir から 一定の範囲角度を付けて拡散します EjectTypeをDIFFに設定したときのみ有効";
                });
        }

        y += height;
        if (!gui_lumi_wave_factor_) {
            gui_lumi_wave_factor_ = Shared<GuiValueSlider< Particle, float >>(
                new GuiValueSlider< Particle, float >
                (this
                    , &Particle::getLuminanceWaveFactor
                    , &Particle::setLuminanceWaveFactor
                    , { pos.x, pos.y + y }
                    , 0.0f
                    , 20.0f
                    , "luminance wave"));
            gui_lumi_wave_factor_->setDescription([&]() {
                description_str_ = "輝度の揺らぎ係数";
                });
        }

        y += height;
        if (!gui_rotate_factor_) {
            gui_rotate_factor_ = Shared<GuiValueSlider< Particle, float >>(
                new GuiValueSlider< Particle, float >
                (this
                    , &Particle::getRotateFactor
                    , &Particle::setRotateFactor
                    , { pos.x, pos.y + y }
                    , 0.0f
                    , 5.0f
                    , "rotate factor"));
            gui_rotate_factor_->setDescription([&]() {
                description_str_ = "回転係数 個々の粒子が指定値を最大としたランダムな回転をします PostureType::UpYD では適用されません";
                });
        }

        y += height;
        if (!gui_distortion_range_) {
            gui_distortion_range_ = Shared<GuiValueSlider< Particle, float >>(
                new GuiValueSlider< Particle, float >
                (this
                    , &Particle::getDistortionRange
                    , &Particle::setDistortionRange
                    , { pos.x, pos.y + y }
                    , 0.0f
                    , 10.0f
                    , "distortion range"));
            gui_distortion_range_->setDescription([&]() {
                description_str_ = "形状の歪み係数 DistortionMode の設定と連携します";
                });
        }

        y += height;
        if (!gui_distortion_velocity_) {
            gui_distortion_velocity_ = Shared<GuiValueSlider< Particle, float >>(
                new GuiValueSlider< Particle, float >
                (this
                    , &Particle::getDistortionVelocity
                    , &Particle::setDistortionVelocity
                    , { pos.x, pos.y + y }
                    , 0.0f
                    , 10.0f
                    , "distortion vlcty"));
            gui_distortion_velocity_->setDescription([&]() {
                description_str_ = "形状の歪みの速さ係数 DistortionMode の設定と連携します";
                });
        }



        gui_play_type_->update();
        gui_blend_mode_->update();
        gui_adulation_mode_->update();
        gui_origin_mode_->update();
        gui_move_surface_->update();
        gui_posture_type_->update();
        gui_disp_size_mode_->update();
        gui_disp_alpha_mode_->update();
        gui_distortion_mode_->update();
        gui_eject_type_->update();

        gui_particle_num_->update();
        gui_time_limit_->update();
        gui_origin_range_->update();
        gui_position_x_->update();
        gui_position_y_->update();
        gui_position_z_->update();
        gui_conv_position_x_->update();
        gui_conv_position_y_->update();
        gui_conv_position_z_->update();

        gui_emissive_->update();
        gui_diff_direction_->update();
        gui_gravity_direction_->update();
        gui_origin_axis_->update();
        gui_time_scale_->update();
        gui_size_x_->update();
        gui_size_y_->update();
        gui_alpha_->update();
        gui_near_alpha_distance_->update();
        gui_start_velocity_->update();
        gui_moving_decay_factor_->update();
        gui_diff_random_angle_range_->update();
        gui_gravity_factor_->update();
        gui_waver_factor_->update();
        gui_lumi_wave_factor_->update();
        gui_rotate_factor_->update();
        gui_distortion_range_->update();
        gui_distortion_velocity_->update();

        if (gui_scale_) gui_scale_->update();
        if (gui_existence_scale_) gui_existence_scale_->update();
        if (gui_disp_alpha_factor_) gui_disp_alpha_factor_->update();

        gui_drag_resouce_->draw();
        gui_play_type_->draw();
        gui_blend_mode_->draw();
        gui_adulation_mode_->draw();
        gui_origin_mode_->draw();
        gui_move_surface_->draw();
        gui_posture_type_->draw();
        gui_distortion_mode_->draw();
        gui_play_type_->draw();
        gui_blend_mode_->draw();
        gui_disp_size_mode_->draw();
        gui_disp_alpha_mode_->draw();
        gui_eject_type_->draw();

        gui_particle_num_->draw();
        gui_time_limit_->draw();
        gui_origin_range_->draw();
        gui_position_x_->draw();
        gui_position_y_->draw();
        gui_position_z_->draw();
        gui_conv_position_x_->draw();
        gui_conv_position_y_->draw();
        gui_conv_position_z_->draw();

        gui_emissive_->draw();
        gui_diff_direction_->draw();
        gui_gravity_direction_->draw();
        gui_origin_axis_->draw();
        gui_time_scale_->draw();
        gui_size_x_->draw();
        gui_size_y_->draw();
        if (gui_scale_) gui_scale_->draw();
        if(gui_existence_scale_) gui_existence_scale_->draw();
        if (gui_disp_alpha_factor_) gui_disp_alpha_factor_->draw();

        gui_alpha_->draw();
        gui_near_alpha_distance_->draw();
        gui_start_velocity_->draw();
        gui_moving_decay_factor_->draw();
        gui_diff_random_angle_range_->draw();
        gui_gravity_factor_->draw();
        gui_waver_factor_->draw();
        gui_lumi_wave_factor_->draw();
        gui_rotate_factor_->draw();
        gui_distortion_range_->draw();
        gui_distortion_velocity_->draw();


        if (!description_str_.empty()) {
            tx = (int)pos_.x;
            ty = DXE_WINDOW_HEIGHT - 20;
            bx = (int)pos_.x + DXE_WINDOW_WIDTH;
            by = DXE_WINDOW_HEIGHT ;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
            DrawBox(tx, ty, bx, by, 0, true);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
            DrawStringEx(tx, ty + 5, -1, "%s", description_str_.c_str());
        }

    }


}

