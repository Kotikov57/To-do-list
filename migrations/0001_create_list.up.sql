create table tasks
(
    id          serial,
    name        text,
    status      text,
    started_at  date,
    finished_at date
);

alter table tasks
    owner to postgres;

create index tasks_id_index
    on tasks (id);