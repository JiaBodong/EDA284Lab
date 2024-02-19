#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <arm_sve.h>
// #include <fomp>
using namespace std::chrono;
using namespace std;

struct moving_particle_type
{
  float x, y, z;
  float velocity_x, velocity_y, velocity_z;
};

void initializeParticles(const int num_of_particles, moving_particle_type *const particles)
{
  #pragma omp parallel for
  for (int i = 0; i < num_of_particles; i++)
  {
    particles[i].x += (float)(rand() % 1000) / 1000;
    particles[i].y += (float)(rand() % 1000) / 1000;
    particles[i].z += (float)(rand() % 1000) / 1000;
    particles[i].velocity_x += (float)(rand() % 1000) / 1000;
    particles[i].velocity_y += (float)(rand() % 1000) / 1000;
    particles[i].velocity_z += (float)(rand() % 1000) / 1000;
  }
}

// void moveParticles(const int num_of_particles, moving_particle_type *const particles, const float dt)
// {
//   //#pragma omp parallel for
//   // loop over the particles that experience force
//   for (int i = 0; i < num_of_particles; i++)
//   {

//     // the gravity force on a particle i on x, y and z
//     float fource_x = 0, fource_y = 0, fource_z = 0;
    
//     // Loop over particles that exert force
//     for (int j = 0; j < num_of_particles; j++)
//     {
//       const float softening = 1e-20;

//       // Newton's law of gravity
//       const float d_x = particles[j].x - particles[i].x;
//       const float d_y = particles[j].y - particles[i].y;
//       const float d_z = particles[j].z - particles[i].z;
//       const float drSquared = d_x * d_x + d_y * d_y + d_z * d_z + softening;
//       const float drPower32 = pow(drSquared, 3.0 / 2.0);

//       // net force calculation
//       fource_x += d_x / drPower32;
//       fource_y += d_y / drPower32;
//       fource_z += d_z / drPower32;
//     }

//     // accelerate the particles as a result of the the gravitational force
//     particles[i].velocity_x += dt * fource_x;
//     particles[i].velocity_y += dt * fource_y;
//     particles[i].velocity_z += dt * fource_z;
//   }

//   // update the particles according to their velocities
//   for (int i = 0; i < num_of_particles; i++)
//   {
//     particles[i].x += particles[i].velocity_x * dt;
//     particles[i].y += particles[i].velocity_y * dt;
//     particles[i].z += particles[i].velocity_z * dt;
//   }
// }


// SVE version
void moveParticles(const int num_of_particles, moving_particle_type *const particles, const float dt)
{
  #pragma omp parallel for
  // loop over the particles that experience force
  for (int i = 0; i < num_of_particles; i++)
  {

    // the gravity force on a particle i on x, y and z
    float force_x = 0, force_y = 0, force_z = 0;
    // set vector to store each i's x,y,z
    svfloat32_t v_particle_i_x = svdup_f32(particles[i].x);
    svfloat32_t v_particle_i_y = svdup_f32(particles[i].y);
    svfloat32_t v_particle_i_z = svdup_f32(particles[i].z);

    svfloat32_t vsoft = svdup_f32(1e-20);
    // const float softening = 1e-20;
    // Loop over particles that exert force
    for (int j = 0; j < num_of_particles; j+=svcntd())
    {
      svbool_t Pg = svwhilelt_b32(j,num_of_particles);//condition v

      float particle_x[svcntd()];
      float particle_y[svcntd()];
      float particle_z[svcntd()];

        // Load each particle's x, y, z components into vectors
      for (int k = 0; k < svcntd(); ++k)
      {
          int idx = j + k;
          if (idx < num_of_particles)
          {
              particle_x[k] = particles[idx].x;
              particle_y[k] = particles[idx].y;
              particle_z[k] = particles[idx].z;
          }
      }
      svfloat32_t v_particle_j_x = svld1(Pg, particle_x);
      svfloat32_t v_particle_j_y = svld1(Pg, particle_y);
      svfloat32_t v_particle_j_z = svld1(Pg, particle_z);

      svfloat32_t vdst_d_x = svsub_x(Pg, v_particle_j_x, v_particle_i_x);
      svfloat32_t vdst_d_y = svsub_x(Pg, v_particle_j_y, v_particle_i_y);
      svfloat32_t vdst_d_z = svsub_x(Pg, v_particle_j_z, v_particle_i_z);

      svfloat32_t vdst_d_x2 = svmul_x(Pg, vdst_d_x, vdst_d_x);
      svfloat32_t vdst_d_y2 = svmul_x(Pg, vdst_d_y, vdst_d_y);
      svfloat32_t vdst_d_z2 = svmul_x(Pg, vdst_d_z, vdst_d_z);

      svfloat32_t vdrSq1 = svadd_x(Pg, vdst_d_x2, vdst_d_y2);
      svfloat32_t vdrSq2 = svadd_x(Pg, vdst_d_z2, vsoft);
      svfloat32_t vdrSq = svadd_x(Pg, vdrSq1, vdrSq2);

      // Compute drPower32 using SVE instructions
      svfloat32_t v_drPower32 = svmul_x(Pg, vdrSq, svdup_f32(3.0 / 2.0));
      v_drPower32 = svsqrt_x(Pg, v_drPower32);
      v_drPower32 = svmul_x(Pg, v_drPower32, v_drPower32);

      svfloat32_t v_force_x = svdiv_x(Pg, vdst_d_x, v_drPower32);
      svfloat32_t v_force_y = svdiv_x(Pg, vdst_d_y, v_drPower32);
      svfloat32_t v_force_z = svdiv_x(Pg, vdst_d_z, v_drPower32);

      svadda(Pg, force_x, v_force_x);
      svadda(Pg, force_y, v_force_y);
      svadda(Pg, force_z, v_force_z);
    }

    // accelerate the particles as a result of the the gravitational force
    particles[i].velocity_x += dt * force_x;
    particles[i].velocity_y += dt * force_y;
    particles[i].velocity_z += dt * force_z;
  }

  // update the particles according to their velocities
  for (int i = 0; i < num_of_particles; i++)
  {
    particles[i].x += particles[i].velocity_x * dt;
    particles[i].y += particles[i].velocity_y * dt;
    particles[i].z += particles[i].velocity_z * dt;
  }
}







void perform_benchmark(const int num_of_particles, const int num_of_steps, moving_particle_type *const particles, const float dt){

  printf("*****************************************************\n");
  printf("Propagating %d particles\n",
         num_of_particles);
  printf("*****************************************************\n");

  double all_steps_duration;
  const int warmup_steps = 2;    // skip first iterations considering them warm-up steps
  printf("\033[1m%5s %10s\033[0m\n", "Step", "Time, s");
  fflush(stdout);
  for (int step = 1; step <= num_of_steps; step++)
  {

    auto start = high_resolution_clock::now();; // start timing
    moveParticles(num_of_particles, particles, dt);
    auto stop = high_resolution_clock::now();; // end timing
    double duration = duration_cast<microseconds>(stop - start).count()/1000000.0;
    all_steps_duration += duration;
    
    printf("%5d %10.3e %s\n",
           step, (duration), (step <= warmup_steps ? "excluded" : ""));
    fflush(stdout);
  }

  double average_step_time = all_steps_duration / num_of_steps;
  double rate = 1 / average_step_time;
  printf("*****************************************************\n");
  printf("\033[1m%s %4s \033[42m%10.1f step/s\033[0m\n",
         "Average performance:", "", rate);
  printf("*****************************************************\n");
}

int main(const int argc, const char **argv)
{
  // Problem size could be passed as an argument
  const int num_of_particles = 256;//default is 2048
  printf("num_of_particles: %d\n", num_of_particles);
  moving_particle_type *particles = static_cast<moving_particle_type*>(aligned_alloc(64, num_of_particles * sizeof(moving_particle_type)));
  // Creating space to store the particles
  // moving_particle_type *particles = new moving_particle_type[num_of_particles];

  // Initialize particles data randomly
  srand(1);
  initializeParticles(num_of_particles, particles);
  const int num_of_steps = 10;  // number of test iterations
  const float dt = 0.01f; // particle propagation step time
  //peform benchmarking
  perform_benchmark(num_of_particles, num_of_steps, particles, dt);
  
  delete particles;
}