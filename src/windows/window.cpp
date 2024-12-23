#include <wincpp/windows/window.hpp>

namespace wincpp::windows
{
    window_t::window_t( const thread_factory& tds, HWND hwnd ) noexcept : tds( tds ), hwnd( hwnd )
    {
    }

    HWND window_t::handle() const noexcept
    {
        return hwnd;
    }

    std::uint32_t window_t::process_id() const
    {
        DWORD pid;
        if ( !GetWindowThreadProcessId( hwnd, &pid ) )
            throw core::error::from_win32( GetLastError() );

        return pid;
    }

    std::optional< window_t > window_t::owner() const noexcept
    {
        const auto owner = GetWindow( hwnd, GW_OWNER );

        if ( !owner )
            return std::nullopt;

        return window_t( tds, owner );
    }

    std::string window_t::title() const
    {
        std::string title;
        title.resize( GetWindowTextLength( hwnd ) + 1 );

        if ( !GetWindowText( hwnd, title.data(), static_cast< int >( title.size() ) ) && GetLastError() )
            throw core::error::from_win32( GetLastError() );

        return title;
    }

    std::string window_t::class_name() const
    {
        std::string class_name;
        class_name.resize( 256 );

        if ( !GetClassName( hwnd, class_name.data(), static_cast< int >( class_name.size() ) ) )
            throw core::error::from_win32( GetLastError() );

        return class_name;
    }

    bool window_t::is_active() const
    {
        return GetForegroundWindow() == hwnd;
    }

    threads::thread_t window_t::thread() const
    {
        if ( const auto& tid = GetWindowThreadProcessId( hwnd, nullptr ) )
            return tds[ tid ];

        throw core::error::from_win32( GetLastError() );
    }

    window_t::placement_t window_t::placement() const
    {
        WINDOWPLACEMENT placement{};
        placement.length = sizeof( WINDOWPLACEMENT );

        if ( !GetWindowPlacement( hwnd, &placement ) )
            throw core::error::from_win32( GetLastError() );

        return placement;
    }

    window_t::placement_t::placement_t( const WINDOWPLACEMENT& placement ) noexcept
        : flags( placement.flags ),
          show_state( static_cast< window_t::state_t >( placement.showCmd ) ),
          min_position( placement.ptMinPosition ),
          max_position( placement.ptMaxPosition ),
          normal_position( placement.rcNormalPosition )
    {
    }

}  // namespace wincpp::windows