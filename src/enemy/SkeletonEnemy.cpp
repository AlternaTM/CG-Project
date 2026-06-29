#include "SkeletonEnemy.h"



SkeletonEnemy::SkeletonEnemy():Enemy(EnemyTipe::Skeleton) {
	init_states();
	life = 255;
    speed = 1.8f;
}

void SkeletonEnemy::init_states() {
    attackState.tot_framex = 13;
    attackState.tot_rows = 4;
    attackState.frame_duration = 0.10f;
    attackState.y_offset = 3;
    attackState.max_frame = 10;


    movingState.tot_framex = 13;
    movingState.tot_rows = 4;
    movingState.frame_duration = 0.8f;
    movingState.y_offset = 0;
    movingState.max_frame = 10;


    currentState = &movingState;
}

void SkeletonEnemy::on_target_in_range() { change_state(&attackState); }
void SkeletonEnemy::on_target_out_of_range() { change_state(&movingState); }