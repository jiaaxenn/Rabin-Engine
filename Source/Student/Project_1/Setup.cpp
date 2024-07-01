#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"

void ProjectOne::setup()
{
    // Create an agent (using the default "Agent::AgentModel::Man" model)
    //auto man = agents->create_behavior_agent("ExampleAgent", BehaviorTreeTypes::Example);

    // You can change properties here or at runtime from a behavior tree leaf node
    // Look in Agent.h for all of the setters, like these:
    // man->set_color(Vec3(1, 0, 1));
    // man->set_scaling(Vec3(7,7,7));
    // man->set_position(Vec3(100, 0, 100));

    // Create an agent with a different 3D model:
    // 1. (optional) Add a new 3D model to the framework other than the ones provided:
    //    A. Find a ".sdkmesh" model or use https://github.com/walbourn/contentexporter
    //       to convert fbx files (many end up corrupted in this process, so good luck!)
    //    B. Add a new AgentModel enum for your model in Agent.h (like the existing Man or Tree).
    // 2. Register the new model with the engine, so it associates the file path with the enum
    //    A. Here we are registering all of the extra models that already come in the package.
    Agent::add_model("Assets\\tree.sdkmesh", Agent::AgentModel::Tree);
    Agent::add_model("Assets\\car.sdkmesh", Agent::AgentModel::Car);
    Agent::add_model("Assets\\bird.sdkmesh", Agent::AgentModel::Bird);
    Agent::add_model("Assets\\ball.sdkmesh", Agent::AgentModel::Ball);
    Agent::add_model("Assets\\hut.sdkmesh", Agent::AgentModel::Hut);
    // 3. Create the agent, giving it the correct AgentModel type.
    //auto tree = agents->create_behavior_agent("ExampleAgent2", BehaviorTreeTypes::Example, Agent::AgentModel::Tree);
    // 4. (optional) You can also set the pitch of the model, if you want it to be rotated differently
    //tree->set_pitch(PI / 2);
    // 5. (optional) Set other aspects to make it start out correctly
    //tree->set_color(Vec3(0, 0.5, 0));   // Set the tree to green

    //Movement tree
    /*auto man = agents->create_behavior_agent("Character", BehaviorTreeTypes::Movement, Agent::AgentModel::Man);
    man->set_position(Vec3(10.f, 0.f, 10.f));*/

    //Avoid tree
    /*auto car = agents->create_behavior_agent("Car", BehaviorTreeTypes::Avoid, Agent::AgentModel::Car);
    car->set_color(Vec3(0.5f, 0, 0));
    car->set_scaling(0.75f);

    auto tree = agents->create_behavior_agent("Tree", BehaviorTreeTypes::Example, Agent::AgentModel::Tree);
    tree->set_pitch(PI / 2);
    tree->set_color(Vec3(0, 0.5f, 0));*/

    //Pause tree
    /*auto bird = agents->create_behavior_agent("Bird", BehaviorTreeTypes::Pause, Agent::AgentModel::Bird);
    bird->set_color(Vec3(0.5f, 0, 0));
    bird->set_scaling(0.025f);
    bird->set_position(Vec3());*/

    //Ball & Player tree
    auto ball1 = agents->create_behavior_agent("ball1", BehaviorTreeTypes::Ball, Agent::AgentModel::Ball);
    ball1->set_color(Vec3(1.0f, 0.5f, 0.4f));
    ball1->set_scaling(0.75f);
    ball1->set_position(Vec3(0.f, 5.f, 0.f));
    ball1->set_movement_speed(35.f);
    auto ball2 = agents->create_behavior_agent("ball2", BehaviorTreeTypes::Ball, Agent::AgentModel::Ball);
    ball2->set_color(Vec3(1.0f, 0.5f, 0.4f));
    ball2->set_scaling(0.75f);
    ball2->set_position(Vec3(25.f, 5.f, 0.f));
    ball2->set_movement_speed(35.f);
    auto ball3 = agents->create_behavior_agent("ball3", BehaviorTreeTypes::Ball, Agent::AgentModel::Ball);
    ball3->set_color(Vec3(1.0f, 0.5f, 0.4f));
    ball3->set_scaling(0.75f);
    ball3->set_position(Vec3(50.f, 5.f, 0.f));
    ball3->set_movement_speed(35.f);
    auto ball4 = agents->create_behavior_agent("ball4", BehaviorTreeTypes::Ball, Agent::AgentModel::Ball);
    ball4->set_color(Vec3(1.0f, 0.5f, 0.4f));
    ball4->set_scaling(0.75f);
    ball4->set_position(Vec3(75.f, 5.f, 0.f));
    ball4->set_movement_speed(35.f);
    auto ball5 = agents->create_behavior_agent("ball5", BehaviorTreeTypes::Ball, Agent::AgentModel::Ball);
    ball5->set_color(Vec3(1.0f, 0.5f, 0.4f));
    ball5->set_scaling(0.75f);
    ball5->set_position(Vec3(100.f, 5.f, 0.f));
    ball5->set_movement_speed(35.f);

    auto player = agents->create_behavior_agent("player", BehaviorTreeTypes::Movement, Agent::AgentModel::Man);
    player->set_position(Vec3(50.f, 0.f, 50.f));
    player->set_movement_speed(75.f);

    // You can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code
    // that yourself (that's the realm of project 2)
    terrain->goto_map(0);

    // You can also enable the pathing layer and set grid square colors as you see fit.
    // Works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
    terrain->pathLayer.set_value(0, 0, Colors::Red);

    // Camera position can be modified from this default
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees

    // Sound control (these sound functions can be kicked off in a behavior tree node - see the example in L_PlaySound.cpp)
    audioManager->SetVolume(0.5f);
    audioManager->PlaySoundEffect(L"Assets\\Audio\\retro.wav");
    // Uncomment for example on playing music in the engine (must be .wav)
    // audioManager->PlayMusic(L"Assets\\Audio\\motivate.wav");
    // audioManager->PauseMusic(...);
    // audioManager->ResumeMusic(...);
    // audioManager->StopMusic(...);
}