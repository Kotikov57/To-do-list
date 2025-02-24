create table tasks
(
    id          serial,
    name        text,
    status      text,
    started_at  timestamp default CURRENT_TIMESTAMP,
    finished_at timestamp
);

alter table tasks
    owner to postgres;

create index tasks_id_index
    on tasks (id);