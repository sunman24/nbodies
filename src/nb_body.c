#include "nb_body.h"

#include <string.h>


void nb_body_init_default(nb_body *const body) 
{
    strncpy(body->name, "\0", 1);
    nb_vector2_init_default(&body->coords);
    nb_vector2_init_default(&body->speed);
    nb_vector2_init_default(&body->force);
    body->mass = 0.0;
    body->radius = 0.0;
}

void nb_body_init(nb_body *const body, const char* name,
    const nb_vector2 *const coords, const nb_vector2 *const speed, 
    const nb_vector2 *const force, nb_float mass, nb_float radius) 
{
    strncpy(body->name, name, NB_NAME_MAX - 1);
    body->name[NB_NAME_MAX - 1] = '\0';

    nb_vector2_copy(&body->coords, coords);
    nb_vector2_copy(&body->speed, speed);
    nb_vector2_copy(&body->force, force);
    body->mass = mass;
    body->radius = radius;
}

void nb_body_copy(nb_body *const body, const nb_body *const copy)
{
    strncpy(body->name, copy->name, NB_NAME_MAX - 1);
    body->name[NB_NAME_MAX - 1] = '\0';

    nb_vector2_copy(&body->coords, &copy->coords);
    nb_vector2_copy(&body->speed, &copy->speed);
    nb_vector2_copy(&body->force, &copy->force);
    body->mass = copy->mass;
    body->radius = copy->radius;
}

const nb_body* nb_body_assign(nb_body *const body, const nb_body *const copy)
{
    if (body == copy)
        return body;
    
    strncpy(body->name, copy->name, NB_NAME_MAX - 1);
    body->name[NB_NAME_MAX - 1] = '\0';

    nb_vector2_assign(&body->coords, &copy->coords);
    nb_vector2_assign(&body->speed, &copy->speed);
    nb_vector2_assign(&body->force, &copy->force);
    body->mass = copy->mass;
    body->radius = copy->radius;

    return body;
}

bool nb_body_read(nb_body *const body, FILE* stream)
{
    char temp_name[NB_NAME_MAX];
    nb_vector2 temp_coords;
    nb_vector2 temp_speed;
    nb_vector2 temp_force;
    nb_float temp_mass;
    nb_float temp_radius;
    bool is_read = true;

    strcpy(temp_name, "\0");
    nb_vector2_init_default(&temp_coords);
    nb_vector2_init_default(&temp_speed);
    nb_vector2_init_default(&temp_force);
    temp_mass = 0.0;
    temp_radius = 0.0;

    // read body name
    int chr = getc(stream);
    for (size_t i = 0; i < NB_NAME_MAX - 1; i++)
    {
        if (chr == '\0' || chr == EOF)
        {
            temp_name[i] = '\0';
            break;
        }
        temp_name[i] = (char)chr;
        
        chr = getc(stream);
    }
    temp_name[NB_NAME_MAX - 1] = '\0';

    // if body name was read
    if (chr == '\0')
    {
        is_read &= nb_vector2_read(&temp_coords, stream);
        is_read &= nb_vector2_read(&temp_speed, stream);
        is_read &= nb_vector2_read(&temp_force, stream);
        is_read &= fread(&temp_mass, sizeof(nb_float), 1, stream) == 1;
        is_read &= fread(&temp_radius, sizeof(nb_float), 1, stream) == 1; 
    }
    else
        is_read = false;

    nb_body_init(body, temp_name, &temp_coords, &temp_speed, &temp_force,
        temp_mass, temp_radius);
    
    return is_read;
}

bool nb_body_write(const nb_body *const body, FILE* stream)
{
    size_t name_len = strlen(body->name) + 1;
    bool is_write = true;

    is_write &= fwrite(body->name, sizeof(char), name_len, stream) == name_len;
    is_write &= nb_vector2_write(&body->coords, stream);
    is_write &= nb_vector2_write(&body->speed, stream);
    is_write &= nb_vector2_write(&body->force, stream);
    is_write &= fwrite(&body->mass, sizeof(nb_float), 1, stream) == 1;
    is_write &= fwrite(&body->radius, sizeof(nb_float), 1, stream) == 1;

    return is_write;
}

bool nb_body_print(const nb_body *const body, FILE* stream)
{
    bool is_print = true;

    is_print &= fprintf(stream, "Body: %s\n", body->name) > 0;
    
    is_print &= fprintf(stream, "Coordinastes = ") > 0;
    is_print &= nb_vector2_print(&body->coords, stream);
    is_print &= fprintf(stream, "\n") > 0;

    is_print &= fprintf(stream, "Speed = ") > 0;
    is_print &= nb_vector2_print(&body->speed, stream);
    is_print &= fprintf(stream, "\n") > 0;

    is_print &= fprintf(stream, "Force = ") > 0;
    is_print &= nb_vector2_print(&body->force, stream);
    is_print &= fprintf(stream, "\n") > 0;

    is_print &= fprintf(stream, "Mass = %lf\n", body->mass) > 0;
    is_print &= fprintf(stream, "Radius = %lf\n", body->radius) > 0;

    return is_print;
}
