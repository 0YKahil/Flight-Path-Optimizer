import React, { useEffect, useMemo, useState } from 'react';
import { createRoot } from 'react-dom/client';
import './styles.css';

const API_BASE_URL = import.meta.env.VITE_API_BASE_URL || 'http://localhost:8080';

async function requestJson(path) {
  const response = await fetch(`${API_BASE_URL}${path}`);
  const data = await response.json();

  if (!response.ok) {
    throw new Error(data.error || data.message || 'Request failed');
  }

  return data;
}

function AirportPicker({ label, value, onChange }) {
  const [query, setQuery] = useState(value?.code || '');
  const [results, setResults] = useState([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');

  useEffect(() => {
    if (value?.code && value.code !== query) {
      setQuery(value.code);
    }
  }, [value]);

  useEffect(() => {
    const trimmed = query.trim();

    if (trimmed.length < 2) {
      setResults([]);
      setError('');
      return;
    }

    const timeout = setTimeout(async () => {
      setLoading(true);
      setError('');

      try {
        const data = await requestJson(`/airports?search=${encodeURIComponent(trimmed)}`);
        setResults(data.airports || []);
      } catch (err) {
        setResults([]);
        setError(err.message);
      } finally {
        setLoading(false);
      }
    }, 220);

    return () => clearTimeout(timeout);
  }, [query]);

  function selectAirport(airport) {
    onChange(airport);
    setQuery(airport.code);
    setResults([]);
  }

  return (
    <section className="fieldGroup">
      <label className="label" htmlFor={`${label}-airport`}>
        {label}
      </label>
      <input
        id={`${label}-airport`}
        className="input"
        value={query}
        onChange={(event) => {
          setQuery(event.target.value);
          onChange(null);
        }}
        placeholder="ICAO or airport name"
      />
      <div className="pickerList" aria-live="polite">
        {loading && <div className="mutedRow">Searching...</div>}
        {error && <div className="errorRow">{error}</div>}
        {!loading &&
          !error &&
          results.slice(0, 5).map((airport) => (
            <button
              className="airportOption"
              key={`${label}-${airport.code}`}
              type="button"
              onClick={() => selectAirport(airport)}
            >
              <span className="airportCode">{airport.code}</span>
              <span className="airportName">{airport.name}</span>
            </button>
          ))}
      </div>
    </section>
  );
}

function App() {
  const [start, setStart] = useState(null);
  const [dest, setDest] = useState(null);
  const [range, setRange] = useState(500);
  const [mode, setMode] = useState(0);
  const [route, setRoute] = useState(null);
  const [status, setStatus] = useState('idle');
  const [error, setError] = useState('');
  const [apiStatus, setApiStatus] = useState('checking');

  const canSubmit = useMemo(() => start?.code && dest?.code && Number(range) > 0, [start, dest, range]);

  useEffect(() => {
    requestJson('/health')
      .then(() => setApiStatus('online'))
      .catch(() => setApiStatus('offline'));
  }, []);

  async function findRoute(event) {
    event.preventDefault();

    if (!canSubmit) {
      return;
    }

    setStatus('loading');
    setError('');
    setRoute(null);

    try {
      const query = new URLSearchParams({
        start: start.code,
        dest: dest.code,
        range: String(range),
        mode: String(mode),
      });
      const data = await requestJson(`/route?${query.toString()}`);
      setRoute(data);
      setStatus('success');
    } catch (err) {
      setError(err.message);
      setStatus('error');
    }
  }

  return (
    <main className="appShell">
      <header className="topBar">
        <div>
          <p className="eyebrow">Flight Path Optimizer</p>
          <h1>Route planner</h1>
        </div>
        <span className={`statusPill ${apiStatus}`}>{apiStatus}</span>
      </header>

      <form className="planner" onSubmit={findRoute}>
        <div className="routeInputs">
          <AirportPicker label="Start" value={start} onChange={setStart} />
          <AirportPicker label="Destination" value={dest} onChange={setDest} />
        </div>

        <section className="controls">
          <label className="label" htmlFor="range">
            Range
          </label>
          <div className="rangeControl">
            <input
              id="range"
              className="rangeInput"
              min="1"
              type="number"
              value={range}
              onChange={(event) => setRange(event.target.value)}
            />
            <span className="unit">nm</span>
          </div>

          <div className="segmented" aria-label="Path display mode">
            <button className={mode === 0 ? 'selected' : ''} type="button" onClick={() => setMode(0)}>
              Codes
            </button>
            <button className={mode === 1 ? 'selected' : ''} type="button" onClick={() => setMode(1)}>
              Names
            </button>
          </div>

          <button className="primaryButton" type="submit" disabled={!canSubmit || status === 'loading'}>
            {status === 'loading' ? 'Finding...' : 'Find route'}
          </button>
        </section>
      </form>

      <section className="resultArea" aria-live="polite">
        {status === 'idle' && <div className="emptyState">Select airports and range.</div>}
        {status === 'error' && <div className="errorState">{error}</div>}
        {route && (
          <div className="routeResult">
            <div className="routeSummary">
              <div>
                <span className="summaryLabel">Distance</span>
                <strong>{Math.round(route.distance)} nm</strong>
              </div>
              <div>
                <span className="summaryLabel">Range</span>
                <strong>{route.rangeNm} nm</strong>
              </div>
              <div>
                <span className="summaryLabel">Stops</span>
                <strong>{Math.max(route.path.length - 2, 0)}</strong>
              </div>
            </div>

            <ol className="pathList">
              {route.path.map((item, index) => (
                <li key={`${item}-${index}`}>
                  <span className="pathNode">{item}</span>
                  {index < route.path.length - 1 && <span className="pathArrow" aria-hidden="true">→</span>}
                </li>
              ))}
            </ol>
          </div>
        )}
      </section>
    </main>
  );
}

createRoot(document.getElementById('root')).render(<App />);
