-- ============================================================
-- Tabla de persistencia para el Piano Arduino
-- Ejecutar esto en Supabase: Dashboard > SQL Editor > New query
-- ============================================================

create table if not exists notas_tocadas (
    id bigserial primary key,
    nota text not null check (nota in ('DO', 'RE', 'MI', 'FA')),
    fecha timestamptz not null default now()
);

-- Índice para que las consultas ordenadas por fecha sean rápidas
create index if not exists idx_notas_tocadas_fecha
    on notas_tocadas (fecha desc);

-- (Opcional pero recomendado) Habilitar Row Level Security
-- y permitir lectura/escritura pública para simplificar el proyecto académico.
-- En un entorno real, esto se restringiría con políticas más estrictas.
alter table notas_tocadas enable row level security;

create policy "Permitir lectura publica"
    on notas_tocadas for select
    using (true);

create policy "Permitir insercion publica"
    on notas_tocadas for insert
    with check (true);
